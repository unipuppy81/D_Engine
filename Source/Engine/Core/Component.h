#pragma once
#include "Engine/Core/Object.h"

class GameObject;
class Transform;

class Component : public Object {
public:
	Component() : Object(ObjectType::Component) {}
	Component(ObjectType type) : Object(type) {}
	virtual ~Component() = default;

	virtual void Init() override {}
	virtual void Update() {}
	virtual void LateUpdate() {}

	// 소유주
	void SetOwner(std::shared_ptr<GameObject> owner) { _owner = owner; }
	std::shared_ptr<GameObject> GetOwner() { return _owner.lock(); }

	// 편의 기능: 본인이 속한 Transform 바로 찾기
	std::shared_ptr<Transform> GetTransform();

protected:
	std::weak_ptr<GameObject> _owner; // 순환 참조 방지 (weak_ptr)
};