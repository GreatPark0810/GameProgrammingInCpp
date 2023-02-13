#pragma once

#include "SpriteComponent.h"
#include <string>
#include <vector>
#include <unordered_map>

class AnimSpriteComponent : public SpriteComponent {
    public:
        // 애니메이션 스프라이트 컴포넌트 클래스 생성자
        AnimSpriteComponent(class Actor *owner, int drawOrder = 100);

        // 프레임에 맞는 텍스쳐를 갱신하여 애니메이션의 토대를 만듬 (Component 부모 클래스에서 오버라이드)
        void Update(float deltaTime) override;
        // 처음 값 설정할 때 사용되는 mAnimTextures Setter
        // mCurrFrame 0으로 초기화 및 mTexture를 애니메이션의 첫번째 프레임으로 설정
        void SetAnimTextures(const std::vector<SDL_Texture*>& textures);

        // 애니메이션 FPS getter
        float GetAnimFPS() const { return mAnimFPS; }
        // 애니메이션 FPS setter
        void SetAnimFPS(float fps) { mAnimFPS = fps; }

    private:
        // 한 애니메이션에 사용되는 모든 텍스쳐들의 집합 벡터
        std::vector<SDL_Texture*> mAnimTextures;
        // 출력중인 현재 (텍스쳐)프레임을 float로 기록
        // 프레임이 시작된 후 경과된 시간을 알아낼 수 있음!
        float mCurrFrame;
        // 애니메이션 프레임 레이트 -> 캐릭터의 속도에 영향을 줌
        float mAnimFPS;
};