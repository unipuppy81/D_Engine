#include "Transform.h"

Transform::Transform() {};
Transform::~Transform() {};

XMMATRIX Transform::GetWorldMatrix() {
	// 1. 크기 행렬
	XMMATRIX matScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);

	// 2. 회전 행렬 (Pitch : X, Yaw : Y, Roll : Z 축 회전)
	// 보통 라디안 값을 받으므로, 필요시 XMConvertToRadians 사용
	XMMATRIX matRotation = XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z);

	// 3. 이동 행렬
	XMMATRIX matTranslation = XMMatrixTranslation(_position.x, _position.y, _position.z);

	// 4. SRT 결합
	XMMATRIX world = matScale * matRotation * matTranslation;

	// 5. 셰이더 전송용 Transpose
	// Transform 클래스에서 미리 뒤집어서 전송 : C++ 은 행 우선, HLSL 은 열 우선
	return XMMatrixTranspose(world);
}