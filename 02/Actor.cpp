#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// public:

Actor::Actor(Game* game) : 
    mState(EActive), mPosition(Vector2::Zero), mScale(1.0f), mRotation(0.0f), mGame(game) {
    // Game 객체의 mActors 또는 mPendingActors 벡터에 이 액터 추가
    mGame->AddActor(this);
}

Actor::~Actor() {
    // Game 객체의 mActors 또는 mPendingActors 벡터에서 이 액터 삭제
    mGame->RemoveActor(this);
    // mComponents 안에 있는 컴포넌트들을 삭제
	// delete 키워드가 Component 객체의 소멸자에서 RemoveComponent() 호출
	// 소멸자를 통해 액터가 사라지면 벡터의 사이즈가 하나 줄어든다.
    while (!mComponents.empty()) {
        delete mComponents.back();
    }
}

void Actor::Update(float deltaTime) {
    // 액터가 EActive 상태일 때 
    // 컴포넌트 업데이트 및 특정 액터 업데이트 함수 호출
    if (mState == EActive) {
        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);
    }
}

void Actor::UpdateComponents(float deltaTime) {
    // mComponents 안에 있는 모든 컴포넌트를 갱신
    for (auto comp : mComponents) {
        comp->Update(deltaTime);
    }
}

void Actor::UpdateActor(float deltaTime) {}

void Actor::AddComponent(Component* component) {
    // 정렬된 컴포넌트 포인터 벡터에서 자리 찾고 insert
    // 순서 : 오름차순
    int myOrder = component->GetUpdateOrder();

    auto iter = mComponents.begin();
    for ( ; iter != mComponents.end(); iter++) {
        if (myOrder < (*iter)->GetUpdateOrder()) {
            break;
        }
    }

    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
    // 찾는 컴포넌트가 mComponents에 존재한다면 erase
    auto iter = std::find(mComponents.begin(), mComponents.end(), component);
    if (iter != mComponents.end()) {
        mComponents.erase(iter);
    }
}

///////////////////////////////////////////////////////////////////////////////
// private: