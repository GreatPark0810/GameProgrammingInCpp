#include "Ship.h"
#include "Game.h"
#include "AnimSpriteComponent.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// public:

Ship::Ship(Game* game) : Actor(game), mRightSpeed(0.0f), mDownSpeed(0.0f) {
    // 생성자 내에서 선언한 변수 asComponent는 생성자가 종료될 때 없어지는가? : Correct!
    // 하지만 SetAnimTextures() 함수로 레퍼런스 파라미터를 전송하고,
    // SetAnimTextures() 함수 내부에서는 레퍼런스가 가리키는 값을 멤버 변수에 복사하므로 문제없다.
    
    // 우주선 텍스처 포인터를 가진 AnimSpriteComponent 생성 후 텍스쳐 설정
    AnimSpriteComponent* asComponent = new AnimSpriteComponent(this);
    std::vector<SDL_Texture*> anims = {
        game->GetTexture(game->GetAbsolutePath() + "Assets/Ship01.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Ship02.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Ship03.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Ship04.png")
    };

    asComponent->SetAnimTextures(anims);
}

void Ship::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);

    // 속도와 deltaTime으로 위치를 갱신
    Vector2 pos = GetPosition();
    pos.x += mRightSpeed * deltaTime;
    pos.y += mDownSpeed * deltaTime;

    // 화면의 좌측 절반으로 위치를 제한 후 SetPosition() 실행
    int WindowWidth = GetGame()->GetWindowWidth();
    int WindowHeight = GetGame()->GetWindowHeight();;

    if (pos.x < WindowWidth / 30) pos.x = WindowWidth / 30;
    else if (pos.x > WindowWidth / 2) pos.x = WindowWidth / 2;

    if (pos.y < WindowHeight / 30) pos.y = WindowHeight / 30;
    else if (pos.y > WindowHeight - WindowHeight/30) pos.y = WindowHeight - WindowHeight/30;

    SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state) {
    mRightSpeed = 0.0f;
    mDownSpeed = 0.0f;

    if (state[SDL_SCANCODE_D]) mRightSpeed += 400.0f;
    else if (state[SDL_SCANCODE_A]) mRightSpeed -= 400.0f;

    if (state[SDL_SCANCODE_S]) mDownSpeed += 300.0f;
    else if (state[SDL_SCANCODE_W]) mDownSpeed -= 300.0f;
}

///////////////////////////////////////////////////////////////////////////////
// private: