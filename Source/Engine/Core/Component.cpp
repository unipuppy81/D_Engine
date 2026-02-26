#include "Engine/Core/Component.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Component/Transform.h"

std::shared_ptr<Transform> Component::GetTransform() {
	auto owner = GetOwner();
	return owner ? owner->GetTransform() : nullptr;
}