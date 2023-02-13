#include "SDL2/SDL_image.h"
#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include <algorithm>
#include <string>

///////////////////////////////////////////////////////////////////////////////
// public:

Game::Game() :
    mRenderer(nullptr), mWindow(nullptr), mIsRunning(true),
    mTicksCount(0), WindowWidth(1024), WindowHeight(768) {}

bool Game::Initialize() {
   	// SDL_Init() : SDL 라이브러리(비디오 & 오디오) 초기화
	// 초기화 실패 시 0이 아닌 정수를 반환
	// 초기화되지 않았다면 SDL 없이 게임을 진행할 수 없으므로 에러 출력 후 false 반환
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
    }

	// 윈도우 생성
	// 윈도우 생성 실패 시 에러 출력 후 false 반환
    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 2)",  // 윈도우 제목
        100,            // 윈도우 좌측 상단 x좌표
		100,			// 윈도우 좌측 상단 y좌표
		GetWindowWidth(),	// 윈도우 width
        GetWindowHeight(), 	// 윈도우 height
		0				// 윈도우 생성 플래그 (0 == 어떠한 플래그도 설정되지 않음)
    );
    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

    // SDL 렌더러 생성
	// 렌더러가 정상적으로 생성되지 않으면 false 반환 후 에러 출력
	mRenderer = SDL_CreateRenderer(
		mWindow,	// SDL 윈도우
		-1,			// 어떤 그래픽카드 사용할 지 (일반적으로 -1)
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // 초기화 플래그
	);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// SDL Image 초기화 : PNG 포맷 지원
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	// 데이터 로드
	LoadData();

	// deltaTime 계산하기 위해 Tick 초기화
	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown() {
	UnloadData();
	IMG_Quit();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	// 현재 액터들을 갱신 중이라면 mPendingActors 벡터에,
	// 그렇지 않다면 mActors 벡터에 추가
	if (mIsUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {
	// 찾는 액터가 mPendingActors 벡터에 존재하면
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		// 맨 끝 액터 포인터와 위치 swap 후 pop_back
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// 찾는 액터가 mActors 벡터에 존재하면
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		// 맨 끝 액터 포인터와 위치 swap 후 pop_back
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	int myOrder = sprite->GetDrawOrder();

	// 그리기 순서에 맞추어 mSprites 벡터 내에 스프라이트 삽입
	auto iter = mSprites.begin();
	for ( ; iter != mSprites.end(); iter++) {
		if (myOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	// erase()는 연산 비용이 많이 들지만
	// mSprites 내 순서를 망치기 때문에 swap 후 pop_back 기법을 실행하지 않는다.
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);

	// 텍스쳐가 이미 맵 상에 존재한다면 그것을 리턴
	if (iter != mTextures.end()) {
		tex = iter->second;
	}
	// 텍스쳐가 맵 상에 존재하지 않는다면 텍스쳐 로딩
	else {
		// 먼저 SDL_Surface를 로드한 이후에 텍스쳐를 로드한다.
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf) {
			SDL_Log("Failed to load texture file: %s", fileName.c_str());
			
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		// surf 변수를 미리 free
		SDL_FreeSurface(surf);
		if (!tex) {
			SDL_Log("Failed to convert surface to texture: %s", fileName.c_str());
			return nullptr;
		}

		// 로드한 텍스쳐를 맵에 emplace
		mTextures.emplace(fileName, tex);
	}

	return tex;
}

///////////////////////////////////////////////////////////////////////////////
// private:

void Game::ProcessInput() {
    // 이벤트 큐에서 막 Dequeue된 이벤트의 정보 저장
	SDL_Event event;
	// 이벤트 큐에 이벤트가 남아있는 한 SDL_PollEvent() 계속 호출
	// SDL_PollEvent() : 이벤트 큐에서 이벤트를 발견하면 true 리턴 후 인자에 이벤트 저장
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			// 유저가 윈도우 창의 X버튼을 클릭하는 이벤트
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}

    // 키보드 상태 배열 얻기
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	// Esc키 눌렀다면 루프 종료
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	// 우주선의 입력 처리
	mShip->ProcessKeyboard(state);
}

void Game::UpdateGame() {
    // 60프레임 고정
	// 마지막 프레임 이후로 16ms가 경과할 때까지 대기
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// deltaTime으로 게임 세계의 오브젝트를 갱신하도록 한다.
	// deltaTime = 현재 프레임 Tick - 이전 프레임 Tick (ms -> s)
	// deltaTime이 매우 커져 게임에 영향을 주지 않도록 0.05s 이하로 설계한다.
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) deltaTime = 0.05f;
    mTicksCount = SDL_GetTicks();

	// mActors에 있는 모든 액터들을 갱신 : Actor가 EActive 상태일 때만 갱신 가능!
	mIsUpdatingActors = true;
	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}
	mIsUpdatingActors = false;

	// 대기 중인 mPendingActors 안에 있는 액터들을 mActors 벡터로 이동
	// 이후 mPendingActors 안에 있는 액터들 삭제
	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 죽은 액터(상태가 EDead인 액터)를 deadActors라는 임시 벡터에 추가 후 제거
	// delete를 통해 Actor 객체 소멸자를 호출한다.
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}
	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::GenerateOutput() {
	// 후면 버퍼 전체를 검정색으로 클리어
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	// 프레임마다 mSprites 벡터에 있는 모든 스프라이트를 그리기
	// mSprites 내의 순서에 따라 그려진다. (화가 알고리즘)
	for (auto sprite : mSprites) {
		sprite->Draw(mRenderer);
	}

	// 지금까지 적용된 렌더링 연산을 갱신
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData() {
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, WindowHeight/2));
	mShip->SetScale(1.5f);

	Actor* gameManager = new Actor(this);
	gameManager->SetPosition(Vector2(GetWindowWidth()/2, GetWindowHeight()/2));

	BGSpriteComponent* bg = new BGSpriteComponent(gameManager);
	bg->SetScreenSize(Vector2(GetWindowWidth(), GetWindowHeight()));
	std::vector<SDL_Texture*> bgTextures = {
		GetTexture(AbsolutePath + "Assets/Farback01.png"), 
		GetTexture(AbsolutePath + "Assets/Farback02.png")
	};
	bg->SetBGTextures(bgTextures);
	bg->SetScrollSpeed(-100.0f);
	
	bg = new BGSpriteComponent(gameManager, 50);
	bg->SetScreenSize(Vector2(GetWindowWidth(), GetWindowHeight()));
	bgTextures = {
		GetTexture(AbsolutePath + "Assets/Stars.png"), 
		GetTexture(AbsolutePath + "Assets/Stars.png")
	};
	bg->SetBGTextures(bgTextures);
	bg->SetScrollSpeed(-200.0f);
}

void Game::UnloadData() {
	// mActors 안에 있는 액터들을 삭제
	// delete 키워드가 Actor 객체의 소멸자에서 RemoveActor() 호출
	// 소멸자를 통해 액터가 사라지면 벡터의 사이즈가 하나 줄어든다.
	while (!mActors.empty()) {
		delete mActors.back();
	}

	for (auto i : mTextures) {
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}