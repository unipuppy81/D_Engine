#pragma once
#include "Engine/Math/Vector.h"
#include "Engine/Math/Matrix.h"
#include "Engine/Core/Component.h"
#include "Engine/Math/Quaternion.h"

using namespace DirectX;

class Transform : public Component {
public:
	Transform();
	virtual ~Transform();

	// Setter
	void SetPosition(const Vector3& pos) { _position = pos; }
	void SetPosition(float x, float y, float z) { _position = { x, y, z }; }

	void SetRotation(const Vector3& rot) { _rotation = rot; }
	void SetRotation(float x, float y, float z) { _rotation = { x,y,z }; }
	void SetRotation(const Quaternion& q) { _rotationQuaternion = q; _useQuaternion = true; }

	void SetScale(const Vector3& scale) { _scale = scale; }
	void SetScale(float x, float y, float z) { _scale = { x,y,z }; }

	void SetParent(Transform* parent) { _parent = parent; }


	// Getter
	Vector3 GetPosition() const { return _position; }
	Vector3 GetRotation() const { return _rotation; }
	Vector3 GetSCale() const { return _scale; }
	Matrix GetWorldMatrix();

	Transform* GetParent() const { return _parent; }

private:
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;

	Quaternion _rotationQuaternion;
	bool _useQuaternion;

	Transform* _parent;
};