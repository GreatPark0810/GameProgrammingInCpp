#include "Component.h"
#include "Actor.h"

///////////////////////////////////////////////////////////////////////////////
// public:

Component::Component(Actor* actor, int updateOrder) :
    mOwner(actor), mUpdateOrder(updateOrder) {
    // 이 컴포넌트를 소유한 액터의 mComponents 벡터에 컴포넌트 포인터 추가
    mOwner->AddComponent(this);
}

Component::~Component() {
    // 이 컴포넌트를 소유한 액터의 mComponents 벡터에서 컴포넌트 포인터 삭제
    mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {}

///////////////////////////////////////////////////////////////////////////////
// private: