#pragma once

#include "Component.h"
#include "SDL2/SDL.h"

class SpriteComponent : public Component {
    public:
        // 스프라이트 컴포넌트 클래스 생성자
        // 스프라이트가 부착될 액터와 그리기 순서값을 매개변수로 받아온다.
        // 그리기 순서값이 작을수록 더욱 뒤쪽에서 그린다. (화가 알고리즘)
        SpriteComponent(class Actor* actor, int drawOrder = 100);
        // 소멸자
        ~SpriteComponent();
        
        // deltaTime으로 컴포넌트 업데이트 (오버라이딩 전용)
        virtual void Update(float deltaTime);
        // 스프라이트 그리기 (Game 클래스의 GenerateOutput()에서 사용)
        virtual void Draw(SDL_Renderer* renderer);
        // 매개변수로 받아온 새로운 텍스쳐를 멤버 변수에 설정 (오버라이딩)
        virtual void SetTexture(SDL_Texture* texture);

        int GetDrawOrder() const { return mDrawOrder; }
        int GetTexWidth() const { return mTexWidth; }
        int GetTexHeight() const { return mTexHeight; }

    protected:
        // 그릴 텍스쳐
        SDL_Texture* mTexture;
        // 화가 알고리즘에서 사용하는 그리기 순서
        int mDrawOrder;

        // 텍스쳐의 가로 길이
        int mTexWidth;
        // 텍스쳐의 세로 길이
        int mTexHeight;
};