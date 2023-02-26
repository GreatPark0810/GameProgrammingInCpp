#include "Player.h"
#include "Game.h"
#include "AnimSpriteComponent.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// public:

Player::Player(Game* game) : Actor(game), mRightSpeed(0.0f), mDownSpeed(0.0f) {    
    // 우주선 텍스처 포인터를 가진 AnimSpriteComponent 생성 후 텍스쳐 설정
    mAnimComponent = new AnimSpriteComponent(this);

    std::vector<SDL_Texture*> textures = {
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character16.png")
    };
    mAnimComponent->InsertAnimation("Idle", textures, true);

    textures = {
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character01.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character02.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character03.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character04.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character05.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character06.png")
    };
    mAnimComponent->InsertAnimation("Move", textures, true);

    textures = {
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character07.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character08.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character09.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character10.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character11.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character12.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character13.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character14.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character15.png"),
    };
    mAnimComponent->InsertAnimation("Jump", textures, false);

    textures = {
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character16.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character17.png"),
        game->GetTexture(game->GetAbsolutePath() + "Assets/Character18.png")
    };
    mAnimComponent->InsertAnimation("Punch", textures, false);
}

void Player::UpdateActor(float deltaTime) {
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

void Player::ProcessKeyboard(const uint8_t* state) {
    mRightSpeed = 0.0f;
    mDownSpeed = 0.0f;

    bool isIdle = true;
    // 키보드를 모두 스캔하여 특정 키가 눌리지 않는 상황일 시 Idle로 판단
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        if (
            state[SDL_SCANCODE_D] != 0 ||
            state[SDL_SCANCODE_A] != 0 ||
            state[SDL_SCANCODE_S] != 0 ||
            state[SDL_SCANCODE_W] != 0 ||
            state[SDL_SCANCODE_SPACE] != 0 ||
            state[SDL_SCANCODE_H] != 0
        ) {
            isIdle = false;
            break;
        }
    }

    if (isIdle) {
        mAnimComponent->SetCurrentAnimation("Idle");
    }

    if (state[SDL_SCANCODE_D]) {
        mAnimComponent->SetCurrentAnimation("Move");
        mRightSpeed += 400.0f;
    }
    else if (state[SDL_SCANCODE_A]) {
        mAnimComponent->SetCurrentAnimation("Move");
        mRightSpeed -= 400.0f;
    }

    if (state[SDL_SCANCODE_S]) {
        mAnimComponent->SetCurrentAnimation("Move");
        mDownSpeed += 300.0f;
    }
    else if (state[SDL_SCANCODE_W]) {
        mAnimComponent->SetCurrentAnimation("Move");
        mDownSpeed -= 300.0f;
    }

    if (state[SDL_SCANCODE_SPACE]) {
        mAnimComponent->SetCurrentAnimation("Jump");
    }

    if (state[SDL_SCANCODE_H]) {
        mAnimComponent->SetCurrentAnimation("Punch");
    }
}

///////////////////////////////////////////////////////////////////////////////
// private: