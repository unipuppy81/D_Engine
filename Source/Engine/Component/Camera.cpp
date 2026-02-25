#include "Camera.h"

Camera::Camera() {}
Camera::~Camera() {}

void Camera::SetLens(float fovY, float aspect, float nearZ, float farZ) {
    _fovY = fovY;
    _aspect = aspect;
    _nearZ = nearZ;
    _farZ = farZ;
}

void Camera::Update() {
    float moveSpeed = 0.05f;

    // 1. 방향 벡터 계산 (바라보는 방향)
    XMVECTOR eye = XMLoadFloat3(&_eye);
    XMVECTOR at = XMLoadFloat3(&_at);
    XMVECTOR up = XMLoadFloat3(&_up);

    XMVECTOR forward = XMVector3Normalize(at - eye); // 앞방향
    XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, forward)); // 우측방향

    // 2. 키보드 입력 체크 (윈도우 API 기준)
    if (::GetAsyncKeyState('W') & 0x8000) eye += forward * moveSpeed; // 앞
    if (::GetAsyncKeyState('S') & 0x8000) eye -= forward * moveSpeed; // 뒤
    if (::GetAsyncKeyState('A') & 0x8000) eye -= right * moveSpeed;   // 좌
    if (::GetAsyncKeyState('D') & 0x8000) eye += right * moveSpeed;   // 우

    // 3. 위치 업데이트 (바라보는 지점 'at'도 같이 이동시켜야 카메라 각도가 고정됨)
    XMStoreFloat3(&_eye, eye);
    XMStoreFloat3(&_at, eye + forward);
}

XMMATRIX Camera::GetViewMatrix() {
    XMVECTOR eye = XMLoadFloat3(&_eye);
    XMVECTOR at = XMLoadFloat3(&_at);
    XMVECTOR up = XMLoadFloat3(&_up);

    XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
    return XMMatrixTranspose(view); // 셰이더용 전치
}

XMMATRIX Camera::GetProjectionMatrix() {
    XMMATRIX proj = XMMatrixPerspectiveFovLH(_fovY, _aspect, _nearZ, _farZ);
    return XMMatrixTranspose(proj); // 셰이더용 전치
}