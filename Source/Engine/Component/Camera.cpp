#include "Camera.h"

Camera::Camera() : Component(ObjectType::Component) {}


void Camera::Update() {
    float moveSpeed = 0.05f;

    XMVECTOR eye = _eye.ToSimd();
    XMVECTOR at = _at.ToSimd();
    XMVECTOR up = _up.ToSimd();

    XMVECTOR forward = XMVector3Normalize(at - eye); // 앞방향
    XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, forward)); // 우측방향

    // 2. 키보드 입력 체크 (윈도우 API 기준)
    if (::GetAsyncKeyState('W') & 0x8000) eye += forward * moveSpeed; // 앞
    if (::GetAsyncKeyState('S') & 0x8000) eye -= forward * moveSpeed; // 뒤
    if (::GetAsyncKeyState('A') & 0x8000) eye -= right * moveSpeed;   // 좌
    if (::GetAsyncKeyState('D') & 0x8000) eye += right * moveSpeed;   // 우

    // 3. 위치 업데이트 (바라보는 지점 'at'도 같이 이동시켜야 카메라 각도가 고정됨)
    _eye = Vector3(eye);
    _at = Vector3(XMVectorAdd(eye, forward));
}

void Camera::SetLens(float fovY, float aspect, float nearZ, float farZ) {
    _fovY = fovY;
    _aspect = aspect;
    _nearZ = nearZ;
    _farZ = farZ;
}

Matrix Camera::GetViewMatrix() {
    Matrix view = XMMatrixLookAtLH(_eye.ToSimd(), _at.ToSimd(), _up.ToSimd());
    return view.Transpose();
}

Matrix Camera::GetProjectionMatrix() {
    Matrix proj = XMMatrixPerspectiveFovLH(_fovY, _aspect, _nearZ, _farZ);
    return proj.Transpose();
}