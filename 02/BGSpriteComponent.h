#pragma once

#include "SpriteComponent.h"
#include "Math.h"
#include <vector>

class BGSpriteComponent : public SpriteComponent {
    public:
        // 배경 스프라이트 컴포넌트 생성자
        BGSpriteComponent(class Actor* owner, int drawOrder = 10);

        // 애니메이션을 프레임마다 갱신 (Component 부모 클래스에서 오버라이드)
        // +) 한 이미지가 화면에서 완전히 벗어나는 시점 고려 -> 배경의 오프셋 갱신하며 이미지 무한 반복
        void Update(float deltaTime) override;
        // 스프라이트 그리기 (SpriteComponent 부모 클래스에서 오버라이드)
        void Draw(SDL_Renderer* renderer) override;

        // 배경 텍스쳐 Setter
        // 오프셋 값을 초기화
        // + 각각의 배경 텍스쳐를 이전 배경 텍스쳐의 오른쪽에 위치시킨 후 mBGTextures 벡터에 emplace_back()
        void SetBGTextures(const std::vector<SDL_Texture*>& textures);
        // 스크린 사이즈 Setter
        void SetScreenSize(const Vector2& size) { mScreenSize = size; }
        // 스크롤 스피드 Setter
        void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
        // 스크롤 스피드 Getter
        float GetScrollSpeed() const { return mScrollSpeed; }

    private:
        // 배경 텍스쳐 + 오프셋 값 캡슐화한 구조체
        struct BGTexture {
            SDL_Texture* mTexture;
            Vector2 mOffset;
        };
        // 배경 텍스쳐 집합의 벡터
        std::vector<BGTexture> mBGTextures;
        // 스크린 사이즈
        Vector2 mScreenSize;
        // 스크롤 스피드
        float mScrollSpeed;
};