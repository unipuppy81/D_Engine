#pragma once
#include "Object.h"
#include <vector>
#include <memory>
#include <algorithm>

class Component;
class Transform;

class GameObject : public Object {
public:
	GameObject();
	virtual ~GameObject();

	// 생명 주기
	virtual void Init() override;
	virtual void Update();
	virtual void LateUpdate();

	// 컴포넌트 관리
	template<typename T>
	std::shared_ptr<T> AddComponent();

	template<typename T>
	std::shared_ptr<T> GetComponent() {
		for (const auto& component : _components) {
			std::shared_ptr<T> target = std::dynamic_pointer_cast<T>(component);
			if (target) return target;
		}
		return nullptr;
	}

	// 기본적으로 모든 오브젝트는 Transform을 가짐
	std::shared_ptr<Transform> GetTransform() { return _transform; }

protected:
	std::shared_ptr<Transform> _transform;
	std::vector<std::shared_ptr<Component>> _components;
};

// 2. 헤더 가장 하단(하지만 파일 끝 내부)에서 구현합니다.
// 이때 Component.h가 포함되어 있어야 SetOwner를 인식합니다.
#include "Component.h"

template<typename T>
inline std::shared_ptr<T> GameObject::AddComponent() {
	std::shared_ptr<T> component = std::make_shared<T>();

	// 이제 Component.h를 include했으므로 SetOwner를 부를 수 있습니다.
	component->SetOwner(std::static_pointer_cast<GameObject>(shared_from_this()));

	_components.push_back(component);
	return component;
}