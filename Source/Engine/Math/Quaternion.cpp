#include "Quaternion.h"
#include "Matrix.h"

Quaternion::Quaternion(FXMVECTOR v) {
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), v);
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(XMQuaternionMultiply(ToSimd(), other.ToSimd()));
}

Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t) {
    return Quaternion(XMQuaternionSlerp(q1.ToSimd(), q2.ToSimd(), t));
}

Quaternion Quaternion::FromEuler(float pitch, float yaw, float roll) {
    return Quaternion(XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}

Matrix Quaternion::ToMatrix() const {
    // 쿼터니언을 회전 행렬로 변환
    return Matrix(XMMatrixRotationQuaternion(ToSimd()));
}