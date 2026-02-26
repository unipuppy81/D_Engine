#pragma once
#include "Matrix.h"
#include "Quaternion.h" 

Matrix Matrix::operator*(const Matrix& other) const {
    return Matrix(XMMatrixMultiply(ToSimd(), other.ToSimd()));
}

Matrix Matrix::Transpose() const {
    return Matrix(XMMatrixTranspose(ToSimd()));
}

Matrix Matrix::Inverse() const {
    XMVECTOR determinant;
    return Matrix(XMMatrixInverse(&determinant, ToSimd()));
}

Matrix Matrix::Identity() {
    return Matrix(XMMatrixIdentity());
}

Matrix Matrix::Translation(float x, float y, float z) {
    return Matrix(XMMatrixTranslation(x, y, z));
}

Matrix Matrix::Translation(const Vector3& v) {
    return Matrix(XMMatrixTranslation(v.x, v.y, v.z));
}

Matrix Matrix::Scaling(float x, float y, float z) {
    return Matrix(XMMatrixScaling(x, y, z));
}

Matrix Matrix::Scaling(const Vector3& v) {
    return Matrix(XMMatrixScaling(v.x, v.y, v.z));
}

Matrix Matrix::RotationX(float angle) { return Matrix(XMMatrixRotationX(angle)); }
Matrix Matrix::RotationY(float angle) { return Matrix(XMMatrixRotationY(angle)); }
Matrix Matrix::RotationZ(float angle) { return Matrix(XMMatrixRotationZ(angle)); }

Matrix Matrix::RotationQuaternion(const Quaternion& q) {
    return Matrix(XMMatrixRotationQuaternion(q.ToSimd()));
}