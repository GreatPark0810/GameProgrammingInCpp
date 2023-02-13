#include "AnimSpriteComponent.h"
#include "Actor.h"

///////////////////////////////////////////////////////////////////////////////
// public:

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder) : 
    SpriteComponent(owner, drawOrder), mCurrFrame(0.0f), mAnimFPS(24.0f) {}

void AnimSpriteComponent::Update(float deltaTime) {
    SpriteComponent::Update(deltaTime);

    if (mAnimTextures.size() > 0) {
        // deltaTime(초 단위)를 이용하여 mCurrFrame을 갱신한다.
        mCurrFrame += mAnimFPS * deltaTime;

        // mCurrFrame이 텍스쳐 벡터의 크기보다 크다면 다시 시작 텍스처로 Replay
        while (mCurrFrame >= mAnimTextures.size()) {
            mCurrFrame -= mAnimTextures.size();
        }

        // 현재 프레임의 텍스쳐를 SpriteComponent 멤버 변수인 mTexture에 업데이트
        SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
    }
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures) {
    // 파라미터로 받아온 텍스쳐 멤버 변수로 복사
    mAnimTextures = textures;

    // 현재 프레임을 0으로 초기화
    // SetTexture() 함수를 통해 애니메이션의 첫번째 프레임 설정
    if (mAnimTextures.size() > 0) {
        mCurrFrame = 0.0f;
        SetTexture(mAnimTextures[0]);
    }
}

///////////////////////////////////////////////////////////////////////////////
// private: