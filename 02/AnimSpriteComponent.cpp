#include "AnimSpriteComponent.h"
#include "Actor.h"
#include <cmath>

///////////////////////////////////////////////////////////////////////////////
// public:

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder) : 
    SpriteComponent(owner, drawOrder), mCurrFrame(0.0f), mAnimFPS(24.0f) {}

void AnimSpriteComponent::Update(float deltaTime) {
    SpriteComponent::Update(deltaTime);

    if ((mCurrAnimation->mAnimTextures).size() > 0) {
        // deltaTime(초 단위)를 이용하여 mCurrFrame을 갱신한다.
        mCurrFrame += mAnimFPS * deltaTime;

        // 현재 애니메이션이 순환하는 애니메이션이라면
        if (mCurrAnimation->mIsReplayingAnim) {
            // mCurrFrame이 텍스쳐 벡터의 크기보다 크다면 다시 시작 텍스처로 Replay
            while (mCurrFrame >= (mCurrAnimation->mAnimTextures).size()) {
                mCurrFrame -= (mCurrAnimation->mAnimTextures).size();
            }
            SetTexture((mCurrAnimation->mAnimTextures)[static_cast<int>(mCurrFrame)]);
        }
        // 현재 애니메이션이 비순환 애니메이션이라면
        else {
            // mCurrFrame이 스프라이트 벡터의 크기보다 크다면 모든 스프라이트를 다 돌았다는 의미
            if (mCurrFrame >= (mCurrAnimation->mAnimTextures).size()) {
                // 재생이 끝나면 Idle 애니메이션으로 다시 돌아가기
                SetCurrentAnimation("Idle");
            }
            else {
                SetTexture((mCurrAnimation->mAnimTextures)[static_cast<int>(mCurrFrame)]);
            }
        }
    }

    // if (mAnimTextures.size() > 0) {
    //     // deltaTime(초 단위)를 이용하여 mCurrFrame을 갱신한다.
    //     mCurrFrame += mAnimFPS * deltaTime;

    //     // mCurrFrame이 텍스쳐 벡터의 크기보다 크다면 다시 시작 텍스처로 Replay
    //     while (mCurrFrame >= mAnimTextures.size()) {
    //         mCurrFrame -= mAnimTextures.size();
    //     }

    //     // 현재 프레임의 텍스쳐를 SpriteComponent 멤버 변수인 mTexture에 업데이트
    //     SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
    // }
}

void AnimSpriteComponent::InsertAnimation(
    const std::string& animName, const std::vector<SDL_Texture*>& textures, bool isReplaying) {

    // Anim 임시 구조체 생성
    Anim anim;
    anim.mAnimTextures = textures;
    anim.mIsReplayingAnim = isReplaying;

    // Idle 애니메이션이 mAnimations 내부에 존재하지 않는다면 이 애니메이션을 Idle로 상정
    const std::string idle = "Idle";
    auto iter = mAnimations.find(idle);

    // Idle 애니메이션이 있다면 Set하지는 않고 그냥 emplace
    if (iter != mAnimations.end()) {
        mAnimations.emplace(animName, anim);
    }
    // Idle 애니메이션이 없다면 mCurrentAnimation으로 Set하여 emplace
    else {
        mAnimations.emplace(idle, anim);
        SetCurrentAnimation(idle);
    }

    // // 파라미터로 받아온 텍스쳐 멤버 변수로 복사
    // mAnimTextures = textures;

    // // 현재 프레임을 0으로 초기화
    // // SetTexture() 함수를 통해 애니메이션의 첫번째 프레임 설정
    // if (mAnimTextures.size() > 0) {
    //     mCurrFrame = 0.0f;
    //     SetTexture(mAnimTextures[0]);
    // }
}

///////////////////////////////////////////////////////////////////////////////
// private: