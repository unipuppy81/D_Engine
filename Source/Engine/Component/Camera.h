#pragma once
#include "Engine/Core/Window.h"
#include "Engine/Core/Component.h"
#include "Engine/Math/Vector.h"
#include "Engine/Math/Matrix.h"
#include "Engine/Math/Quaternion.h"

using namespace DirectX;

class Camera : public Component {
public:
	Camera();
	virtual ~Camera() = default;

	virtual void Init() override {}
	virtual void Update() override;
	virtual void LateUpdate() override {}

	// Setter
	// 카메라 위치/방향 설정
	void SetEye(float x, float y, float z) { _eye = { x, y, z }; }
	void SetAt(float x, float y, float z) { _at = { x, y, z }; }
	// 화면 설정 (Projection)
	void SetLens(float fovY, float aspect, float nearZ, float farZ);
	
	
	// Getter
	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix();

private:
	// 뷰 행렬용 데이터
	Vector3 _eye = { 0.0f, 0.0f, -5.0f };	// 카메라 위치
	Vector3 _at = { 0.0f, 0.0f, 0.0f };	// 바라보는 지점
	Vector3 _up = { 0.0f, 1.0f, 0.0f };	// 하늘 방향

	// 투영 데이터
	float _fovY = XM_PIDIV2;
	float _aspect = 1280.0f / 720.0f;
	float _nearZ = 0.1f;
	float _farZ = 1000.0f;
};