#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Transform {
public:
	Transform();
	~Transform();

	// 데이터 설정 (Setter)
	void SetPosition(const XMFLOAT3& pos) { _position = pos; }
	void SetPosition(float x, float y, float z) { _position = { x, y, z }; }

	void SetRotation(const XMFLOAT3& rot) { _rotation = rot; }
	void SetRotation(float x, float y, float z) { _rotation = { x,y,z }; }

	void SetScale(const XMFLOAT3& scale) { _scale = scale; }
	void SetScale(float x, float y, float z) { _scale = { x,y,z }; }

	// 데이터 가져오기 (Getter)
	XMFLOAT3 GetPosition() const { return _position; }
	XMFLOAT3 GetRotation() const { return _rotation; }
	XMFLOAT3 GetSCale() const { return _scale; }

	// World 행렬 계산 및 반환
	XMMATRIX GetWorldMatrix();

private:
	XMFLOAT3 _position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 _rotation = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 _scale = { 1.0f, 1.0f, 1.0f };
};