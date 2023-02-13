#include "BGSpriteComponent.h"
#include "Actor.h"

///////////////////////////////////////////////////////////////////////////////
// public:

BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder) :
    SpriteComponent(owner, drawOrder), mScrollSpeed(0.0f) {}

void BGSpriteComponent::Update(float deltaTime) {
    SpriteComponent::Update(deltaTime);

    for (auto& bg : mBGTextures) {
        // x 오프셋 값을 갱신
        // mScrollSpeed가 음수이므로 x 오프셋을 감소시켜 텍스처가 왼쪽으로 이동하게끔 함
        bg.mOffset.x += mScrollSpeed * deltaTime;

        // 텍스쳐가 화면을 완전히 벗어나면 
        // 오프셋을 마지막 배경 텍스쳐의 오른쪽 위치로 초기화
        if (bg.mOffset.x < -mScreenSize.x) {
            bg.mOffset.x = (mBGTextures.size()-1) * mScreenSize.x - 1;
        }
    }
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer) {
    // mBGTextures 벡터 내의 배경 텍스쳐를 for loop를 통해 모두 그리기
    for (auto& bg : mBGTextures) {
        SDL_Rect r;

        r.w = static_cast<int>(mScreenSize.x);
        r.h = static_cast<int>(mScreenSize.y);
        // mOwner의 위치 중앙에 SDL_Rect(배경) 고정시키기 + 오프셋 적용
        r.x = static_cast<int>(mOwner->GetPosition().x - (r.w / 2) + bg.mOffset.x);
        r.y = static_cast<int>(mOwner->GetPosition().y - (r.h / 2) + bg.mOffset.y);

        // SDL_RenderCopy()를 이용하여 배경 그리기
        SDL_RenderCopy(renderer, bg.mTexture, nullptr, &r);
    }
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures) {
    int count = 0;
    for (auto tex : textures) {
        BGTexture temp;

        temp.mTexture = tex;
        // 각 텍스쳐의 오프셋 = 화면 너비 * count
        temp.mOffset.x = count * mScreenSize.x;
        temp.mOffset.y = 0;

        // 오프셋을 설정한 텍스쳐를 mBGTextures 벡터에 emplace_back
        mBGTextures.emplace_back(temp);
        count++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// private: