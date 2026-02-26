#include "Transform.h"

Transform::Transform() {};
Transform::~Transform() {};

Matrix Transform::GetWorldMatrix() {
	// Scaling Matrix
	Matrix s = Matrix::Scaling(_scale);

	// Rotation Matrix
	Matrix r = _useQuaternion ? _rotationQuaternion.ToMatrix() :
		(Matrix::RotationX(_rotation.x) * Matrix::RotationY(_rotation.y) * Matrix::RotationZ(_rotation.z));

	// Translation Matrix
	Matrix t = Matrix::Translation(_position);

	Matrix local = s * r * t;

	if (_parent) {
		return local * _parent->GetWorldMatrix();
	}

	return local;
}