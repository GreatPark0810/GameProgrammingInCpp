#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
// public:

SpriteComponent::SpriteComponent(Actor* actor, int drawOrder) :
    Component(actor), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0) {
    // 게임 클래스의 mSprites 벡터에 스프라이트 추가
    mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
    // 게임 클래스의 mSprites 벡터에서 기존 스프라이트 제거
    mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Update(float deltaTime) {}

void SpriteComponent::Draw(SDL_Renderer* renderer) {
    SDL_Rect r;
    // 소유 액터의 스케일로 너비와 높이를 맞추기
    // 소유 액터의 위치를 중심으로 사각형 배치
    r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
    r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
    r.x = static_cast<int>(mOwner->GetPosition().x - r.w/2);
    r.y = static_cast<int>(mOwner->GetPosition().y - r.h/2);

    // 사각형 영역 안에 스프라이트 그리기
    SDL_RenderCopyEx(
        renderer,       // 렌더러
        mTexture,       // 그릴 텍스쳐
        nullptr,        // 그릴 텍스처의 영역(nullptr면 전체)
        &r,             // 그려질 스프라이트의 사각형 영역
        -Math::RadiansToDegrees(mOwner->GetRotation()), // 라디안을 각도로 변환
        // SDL에서의 양의 각도 : 시계방향 -> 단위원과 다르므로, 음수로 각도 반전
        nullptr,        // 회전 중심점 (중심이면 nullptr)
        SDL_FLIP_NONE   // 텍스쳐 플립 (대개 SDL_FLIP_NONE)
    );
}

void SpriteComponent::SetTexture(SDL_Texture* texture) {
    mTexture = texture;
    // 텍스쳐의 너비와 높이를 구하여 멤버 변수에 저장
    SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

///////////////////////////////////////////////////////////////////////////////
// protected: