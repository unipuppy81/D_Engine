#include "Engine/Core/GameObject.h"
#include "Engine/Core/Component.h"
#include "Engine/Component/Transform.h"

GameObject::GameObject() : Object(ObjectType::GameObject){ 

}

GameObject::~GameObject() {
	_components.clear();
}

void GameObject::Init() {
	_transform = AddComponent<Transform>();

	for (auto& component : _components) {
		component->Init();
	}
}

void GameObject::Update() {
	for (auto& component : _components) {
		component->Update();
	}
}

void GameObject::LateUpdate() {
	for (auto& component : _components) {
		component->LateUpdate();
	}
}

