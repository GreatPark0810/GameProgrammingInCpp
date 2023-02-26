#pragma once

#include "SDL2/SDL.h"
#include <vector>
#include <unordered_map>

class Game {
    public:
        // Game 클래스 생성자
		Game();

		// 게임 처음 실행 시 초기화
		bool Initialize();
		// 게임 실행 중 게임 루프 실행
		void RunLoop();
		// 게임 종료 (Initialize() 거꾸로)
		void Shutdown();
        
        // 새로운 액터를 mActors 벡터나 mPendingActors 벡터에 추가
        void AddActor(class Actor* actor);
        // mActors 벡터나 mPendingActors 벡터에서 액터 제거
        void RemoveActor(class Actor* actor);

        // 새로운 스프라이트를 mSprites 벡터에 추가
        // mSprites는 그리는 순서로 정렬되어 있어야 한다 (오름차순)
        void AddSprite(class SpriteComponent* sprite);
        // mSprites 벡터에서 스프라이트 제거
        void RemoveSprite(class SpriteComponent* sprite);

        // fileName을 통해 텍스쳐를 로딩 후 텍스쳐의 포인터 반환
        SDL_Texture* GetTexture(const std::string& fileName);

        // 윈도우 가로 길이 Getter
        const int GetWindowWidth() { return WindowWidth; }
        // 윈도우 세로 길이 Getter
        const int GetWindowHeight() { return WindowHeight; }
        // 절대 경로 Getter
        const std::string GetAbsolutePath() { return AbsolutePath; }

    private:
        // 게임 루프 실행 중 외부 입력 처리
		void ProcessInput();
		// 외부 입력을 통해 게임 루프 갱신
		void UpdateGame();
		// 갱신된 결괏값 출력
		void GenerateOutput();

        // 게임 내에 액터 생성
        void LoadData();
        // 게임 종료 전 데이터 정리
        void UnloadData();

        // 게임 클래스 내에서 활성화 중인 액터
        std::vector<class Actor*> mActors;
        // 활성화 전 대기 중인 액터 (임시로 보관 역할)
        std::vector<class Actor*> mPendingActors;
        // 현재 액터들을 갱신 중인지 판단
        bool mIsUpdatingActors;

        // 텍스처를 저장하기 위한 unordered_map
        // Key: 파일 이름 / Value: SDL_Texture *
        std::unordered_map<std::string, SDL_Texture*> mTextures;
        // 그려질 스프라이트를 저장하기 위한 벡터
        std::vector<class SpriteComponent*> mSprites;

		// 그래픽 표현을 위한 SDL_Renderer
		SDL_Renderer *mRenderer;
		// SDL 생성 윈도우
		SDL_Window *mWindow;

		// 게임이 계속 실행되는 지 판단
		bool mIsRunning;
		// 게임 시간이 얼마나 흘렀는지 판단(밀리초)
		Uint32 mTicksCount;

        // 게임 내의 우주선 오브젝트
        class Player* mPlayer;

        // 윈도우 창 가로 픽셀
        const int WindowWidth;
        // 윈도우 창 세로 픽셀
        const int WindowHeight;
        // 현재 프로젝트의 절대 경로
        const std::string AbsolutePath = "D:/Github/Game Programming In C++/02/";
};