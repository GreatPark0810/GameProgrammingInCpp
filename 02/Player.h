#pragma once

#include "Actor.h"
#include <stdint.h>

class Player : public Actor {
    public:
        // Ship 오브젝트 생성자
        Player(class Game* game);
        // Ship 액터에 특화된 업데이트 코드 (Actor 클래스에서 오버라이드)
        void UpdateActor(float deltaTime) override;
        // 키보드 입력 처리 함수
        void ProcessKeyboard(const uint8_t* state);

        // 오른쪽(+) 속도 Getter
        float GetRightSpeed() const { return mRightSpeed; }
        // 아래쪽(+) 속도 Getter
        float GetDownSpeed() const { return mDownSpeed; }

    private:
        // 좌(-)우(+) 이동 속도
        float mRightSpeed;
        // 상(-)하(+) 이동 속도
        float mDownSpeed;
        
        // 애니메이션 컴포넌트를 가리키는 포인터
        class AnimSpriteComponent* mAnimComponent;
};