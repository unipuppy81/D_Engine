#include "Camera.h"

Camera::Camera() {}
Camera::~Camera() {}

void Camera::SetLens(float fovY, float aspect, float nearZ, float farZ) {
    _fovY = fovY;
    _aspect = aspect;
    _nearZ = nearZ;
    _farZ = farZ;
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