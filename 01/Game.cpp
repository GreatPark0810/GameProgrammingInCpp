#include "Game.h"
#define EPSILON 0.0001
///////////////////////////////////////////////////////////////////////////////
// public:

Game::Game() : 
	m_renderer(nullptr), m_window(nullptr), m_is_running(true), 
	m_ticks_count(0) {}

bool Game::Initialize() {
	// SDL_Init() : SDL 비디오 서브시스템 초기화
	// 초기화 실패 시 0이 아닌 정수를 반환
	// 초기화되지 않았다면 SDL 없이 게임을 진행할 수 없으므로 false 반환 후 에러 출력
    int sdl_result = SDL_Init(SDL_INIT_VIDEO);

	if (sdl_result != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
    }

	// SDL 윈도우 생성
	// 윈도우가 정상적으로 생성되지 않으면 false 반환 후 에러 출력
	m_window = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // 윈도우 제목
		100,			// 윈도우 좌측 상단 x좌표
		100,			// 윈도우 좌측 상단 y좌표
		window_width,	// 윈도우 width
		window_height, 	// 윈도우 height
		0				// 윈도우 생성 플래그 (0 == 어떠한 플래그도 설정되지 않음)
	);

	if (!m_window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// SDL 렌더러 생성
	// 렌더러가 정상적으로 생성되지 않으면 false 반환 후 에러 출력
	m_renderer = SDL_CreateRenderer(
		m_window,	// SDL 윈도우
		-1,			// 어떤 그래픽카드 사용할 지 (일반적으로 -1)
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // 초기화 플래그
	);

	if (!m_renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
	}

	// 공과 패들 벡터에 push_back
	m_balls.push_back(Ball(
		Vector2(thickness, thickness), 
		Vector2(window_width / 2.0f, window_height * 0.3f),
		Vector2(-200.0f, 250.0f)
	));
	m_balls.push_back(Ball(
		Vector2(thickness, thickness), 
		Vector2(window_width / 2.0f, window_height * 0.7f),
		Vector2(250.0f, -200.0f)
	));

	m_paddles.push_back(Paddle(
		Vector2(thickness, 100.0f),
		Vector2(thickness / 2.0f, window_height / 2.0f),
		300.0f,
		0
	));
	m_paddles.push_back(Paddle(
		Vector2(thickness, 100.0f),
		Vector2(window_width - (thickness / 2.0f), window_height / 2.0f),
		300.0f,
		0
	));

	return true;
}

void Game::RunLoop() {
	// 게임 종료 신호를 받을 때까지 계속 반복
    while (m_is_running) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() {
	// 렌더러 및 윈도우 객체 해제 후 SDL 종료
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
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
			// 유저가 윈도우를 닫는 이벤트
			case SDL_QUIT:
				m_is_running = false;
				break;
		}
	}

	// 키보드 상태 배열 얻기
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	// Esc키 눌렀다면 루프 종료
	if (state[SDL_SCANCODE_ESCAPE]) {
		m_is_running = false;
	}

	// 패들 방향 입력값 받기
	// 대입 연산자를 통해 W키와 S키를 동시에 누르면 m_paddles[i].direction가 0임을 보장
	m_paddles[0].direction = m_paddles[1].direction = 0;
	
	if (state[SDL_SCANCODE_W]) {
		m_paddles[0].direction -= 1;
	}
	else if (state[SDL_SCANCODE_S]) {
		m_paddles[0].direction += 1;
	}
	else if (state[SDL_SCANCODE_UP]) {
		m_paddles[1].direction -= 1;
	}
	else if (state[SDL_SCANCODE_DOWN]) {
		m_paddles[1].direction += 1;
	}
}

void Game::UpdateGame() {
	// 60프레임 고정
	// 마지막 프레임 이후로 16밀리초가 경과할 때까지 대기
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticks_count + 16));

	// delta_time으로 게임 세계의 오브젝트를 갱신하도록 한다.
	// delta_time = 현재 프레임 Tick - 이전 프레임 Tick (밀리초 -> 초)
	// delta_time이 매우 커져 게임에 영향을 주지 않도록 0.05초 이하로 설계한다.
	float delta_time = (SDL_GetTicks() - m_ticks_count) / 1000.0f;
	if (delta_time > 0.05f) delta_time = 0.05f;
	m_ticks_count = SDL_GetTicks();

	for (Paddle &paddle : m_paddles) {
		// 패들 이동 갱신
		if (paddle.direction != 0) {
			paddle.pos.y += paddle.direction * paddle.speed * delta_time;

			// 패들이 위아래로 움직일 수 있는 최댓값 제한
			if (paddle.pos.y < paddle.scale.y / 2.0f + thickness) {
				paddle.pos.y = paddle.scale.y / 2.0f + thickness;
			} 
			else if (paddle.pos.y > window_height - (paddle.scale.y / 2.0f) - thickness) {
				paddle.pos.y = window_height - (paddle.scale.y / 2.0f) - thickness;
			} 
		}
	}

	for (Ball &ball : m_balls) {
		// 공의 이동 갱신
		ball.pos.x += ball.velocity.x * delta_time;
		ball.pos.y += ball.velocity.y * delta_time;

		// 공이 만약 위쪽 또는 아래쪽 벽에 충돌했다면 공의 y속도를 반전
		if (ball.pos.y <= thickness + (thickness * 0.5f) && 
			ball.velocity.y < 0.0f) {
			ball.velocity.y *= -1.0f;
		}
		else if (ball.pos.y >= window_height - (thickness + (thickness * 0.5f)) && 
			ball.velocity.y > 0.0f) {
			ball.velocity.y *= -1.0f;
		}

		// 패들과 공의 충돌 검사
		// 공의 x 이동 방향에 따라 어느 패들을 대상으로 하는지 파악
		float ypos_diff;

		// 공이 negative x 방향으로 이동하는 경우 왼쪽 패들을 대상으로 한다.
		if (ball.velocity.x < 0.0f) {
			Paddle &paddle = m_paddles[0];
			ypos_diff = std::abs(paddle.pos.y - ball.pos.y);

			if (ypos_diff <= (paddle.scale.y + ball.scale.y) / 2.0f &&
				ball.pos.x <= paddle.pos.x + (paddle.scale.x + ball.scale.x) / 2.0f) {
				ball.velocity.x *= -1.0f;
			}
		}
		// 공이 positive x 방향으로 이동하는 경우 오른쪽 패들을 대상으로 한다.
		else if (ball.velocity.x > 0.0f) {
			Paddle &paddle = m_paddles[1];
			ypos_diff = std::abs(paddle.pos.y - ball.pos.y);

			if (ypos_diff <= (paddle.scale.y + ball.scale.y) / 2.0f &&
				ball.pos.x >= paddle.pos.x - (paddle.scale.x + ball.scale.x) / 2.0f) {
				ball.velocity.x *= -1.0f;
			}
		}

		// 공이 화면 왼쪽 끝이나 오른쪽 끝에 도달했다면 게임 오버
		if (ball.pos.x <= 0.0f || ball.pos.x >= window_width) {
			m_is_running = false;
		}
	}
}

void Game::GenerateOutput() {
	// 후면 버퍼 전체를 검정색으로 클리어
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	// 벽 그리기 위해 색상 변경
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	// 위쪽 벽 그리기
	SDL_Rect top_wall { 0, 0, window_width, thickness };
	SDL_RenderFillRect(m_renderer, &top_wall);

	// 아래쪽 벽 그리기
	SDL_Rect bottom_wall { 0, window_height - thickness, window_width, thickness };
	SDL_RenderFillRect(m_renderer, &bottom_wall);

	for (Ball &ball : m_balls) {
		// 공 그리기
		SDL_Rect draw_ball {
			static_cast<int>(ball.pos.x - ball.scale.x/2.0f),
			static_cast<int>(ball.pos.y - ball.scale.y/2.0f),
			static_cast<int>(ball.scale.x),
			static_cast<int>(ball.scale.y)
		};
		SDL_RenderFillRect(m_renderer, &draw_ball);
	}
	for (Paddle &paddle : m_paddles) {
		// 패들 그리기
		SDL_Rect draw_paddle {
		static_cast<int>(paddle.pos.x - paddle.scale.x/2.0f),
		static_cast<int>(paddle.pos.y - paddle.scale.y/2.0f),
		static_cast<int>(paddle.scale.x),
		static_cast<int>(paddle.scale.y)
		};
		SDL_RenderFillRect(m_renderer, &draw_paddle);
	}
	

	// 전면 버퍼와 후면 버퍼를 교환하여 그래픽 표시
	SDL_RenderPresent(m_renderer);
}