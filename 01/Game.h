#include "SDL2/SDL.h"
#include <vector>
#include <cmath>

const int window_width = 1024;
const int window_height = 768;

const int thickness = 15;

struct Vector2 {
	float x;
	float y;

	Vector2() : x(0.0f), y(0.0f) {} 
	Vector2 (float x, float y) : x(x), y(y) {}
};

struct Ball {
	Vector2 scale;
	Vector2 pos;
	Vector2 velocity;

	Ball() {}
	Ball(Vector2 scale, Vector2 pos, Vector2 velocity) :
		scale(scale), pos(pos), velocity(velocity) {}
};

struct Paddle {
	Vector2 scale;
	Vector2 pos;
	float speed;
	int direction;

	Paddle() {}
	Paddle(Vector2 scale, Vector2 pos, float speed, int direction) :
		scale(scale), pos(pos), speed(speed), direction(direction) {} 
};

class Game {
	public:
		// Game 클래스 생성자
		Game();

		// 게임 처음 실행 시 초기화
		bool Initialize();
		// 게임 실행 중 게임 루프 실행
		void RunLoop();
		// 게임 종료
		void Shutdown();

	private:
		// 게임 루프 실행 중 외부 입력 처리
		void ProcessInput();
		// 외부 입력을 통해 게임 루프 갱신
		void UpdateGame();
		// 갱신된 결괏값 출력
		void GenerateOutput();

		// 그래픽 표현을 위한 SDL_Renderer
		SDL_Renderer *m_renderer;
		// SDL 생성 윈도우
		SDL_Window *m_window;

		// 게임이 계속 실행되는 지 판단
		bool m_is_running;
		// 게임 시간이 얼마나 흘렀는지 판단(밀리초)
		Uint32 m_ticks_count;

		// 공 구조체를 담는 벡터
		std::vector<Ball> m_balls;
		// 패들 구조체를 담는 벡터
		std::vector<Paddle> m_paddles;
};