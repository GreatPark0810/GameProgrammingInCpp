#pragma once

#include "SpriteComponent.h"
#include <string>
#include <vector>
#include <unordered_map>

// 한 애니메이션의 텍스쳐 묶음
// mIsReplayingAnim : 순환되는 애니메이션인지 판단
struct Anim {
    std::vector<SDL_Texture*> mAnimTextures;
    bool mIsReplayingAnim;
};

class AnimSpriteComponent : public SpriteComponent {
    public:
        // 애니메이션 스프라이트 컴포넌트 클래스 생성자
        AnimSpriteComponent(class Actor *owner, int drawOrder = 100);

        // 프레임에 맞는 텍스쳐를 갱신하여 애니메이션의 토대를 만듬 (Component 부모 클래스에서 오버라이드)
        void Update(float deltaTime) override;

        // 애니메이션을 받아와서 mAnimation 맵에 삽입
        void InsertAnimation(
            const std::string& animName, const std::vector<SDL_Texture*>& textures, bool isReplaying);

        // mCurrentAnimation Getter
        Anim* GetCurrentAnimation() { return mCurrAnimation; }

        // mCurrentAnimation Setter (애니메이션 이름 변수로 받아와서 map 내에서 찾고 이후 Set)
        void SetCurrentAnimation(const std::string& animName) {
            auto iter = mAnimations.find(animName);
            
            if (iter != mAnimations.end()) {
                // 만약 현재 재생 중인 애니메이션이 iter가 가리키는 것과 일치하다면 함수 실행 X
                if (mCurrAnimation == &(iter->second)) return;

                mCurrFrame = 0.0f;
                mCurrAnimation = &(iter->second);
                SetTexture(((iter->second).mAnimTextures)[0]);
            }
            else {
                SDL_Log("There is no animation in mAnimations map...");
            }
        }

        // 애니메이션 FPS getter
        float GetAnimFPS() const { return mAnimFPS; }
        // 애니메이션 FPS setter
        void SetAnimFPS(float fps) { mAnimFPS = fps; }

    private:
        // 여러 개의 다양한 애니메이션을 저장하는 map
        // std::string : 애니메이션의 이름 (Idle, Move, Jump, ...)
        // Anim : 각 애니메이션의 텍스쳐 묶음
        std::unordered_map<std::string, Anim> mAnimations;
        // 현재 재생되고 있는 애니메이션의 포인터
        Anim* mCurrAnimation;

        // 출력중인 현재 (텍스쳐)프레임을 float로 기록
        // 프레임이 시작된 후 경과된 시간을 알아낼 수 있음!
        float mCurrFrame;
        // 애니메이션 프레임 레이트 -> 캐릭터의 속도에 영향을 줌
        float mAnimFPS;
};