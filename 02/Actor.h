#pragma once

#include <vector>
#include "Math.h"

class Actor {
    public:
        // 액터의 상태 추적
        enum State {
            EActive,
            EPaused,
            EDead
        };

        // 액터 생성자
        Actor(class Game* game);
        // 액터 소멸자 (오버라이딩 가능)
        virtual ~Actor();

        // Game 클래스에서 호출하는 Update 함수 (가상 함수 X)
        // mActors 또는 mPendingActors 벡터 안에 있는 액터들을 iterate하며 각각 갱신
        void Update(float deltaTime);
        // 액터에 추가되어 있는 모든 컴포넌트를 업데이트 (가상 함수 X)
        void UpdateComponents(float deltaTime);
        // 특정 액터에 특화된 업데이트 코드 (오버라이딩 전용)
        virtual void UpdateActor(float deltaTime);

        // 액터에 새로운 컴포넌트 추가
        void AddComponent(class Component* component);
        // 액터에 기존 컴포넌트 제거
        void RemoveComponent(class Component* component);

        ///////////////////////////////////////////////////////////////////////
        // Getter / Setter
        
        State GetState() const { return mState; }
        void SetState(State s) { mState = s; }

        const Vector2& GetPosition() const { return mPosition; }
        void SetPosition(float x, float y) { mPosition.SetValue(x, y); }
        void SetPosition(const Vector2& pos) { mPosition = pos; }

        float GetScale() const { return mScale; }
        void SetScale(float s) { mScale = s; }

        float GetRotation() const { return mRotation; }
        void SetRotation(float r) { mRotation = r; }

        class Game* GetGame() const { return mGame; }

    private:
        // 액터의 현재 상태
        State mState;

        // 액터의 중심점
        Vector2 mPosition;
        // 액터의 배율 (100% -> 1.0f)
        float mScale;
        // 액터의 회전 각도 (라디안)
        float mRotation;

        // 액터가 보유한 컴포넌트의 포인터를 vector 컨테이너로 저장
        std::vector<class Component*> mComponents;
        // Game 클래스에 접근하기 위한 포인터
        class Game* mGame;
};