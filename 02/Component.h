#pragma once

class Component {
    public:
        // 컴포넌트 클래스 생성자
        // 컴포넌트가 부착될 액터와 업데이트 순서값을 매개변수로 받아온다.
        // 업데이트 순서값이 작을수록, 컴포넌트는 더욱 빨리 갱신된다.
        Component(class Actor* actor, int updateOrder = 100);
        // 소멸자 (오버라이딩 가능)
        virtual ~Component();
        // deltaTime으로 컴포넌트 업데이트 (오버라이딩 전용)
        virtual void Update(float deltaTime);
        
        // 업데이트 순서 getter
        int GetUpdateOrder() const { return mUpdateOrder; }

    protected:
        // 이 컴포넌트를 소유하고 있는 액터
        class Actor* mOwner;
        // 액터 내에서 컴포넌트의 업데이트 순서
        int mUpdateOrder;
};