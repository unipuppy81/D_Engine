#pragma once
#include "Vector.h"

struct Matrix {
    union {
        XMFLOAT4X4 m;
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
    };

    // 생성자는 짧으니 헤더에 둡니다
    Matrix() { XMStoreFloat4x4(&m, XMMatrixIdentity()); }
    Matrix(FXMMATRIX mat) { XMStoreFloat4x4(&m, mat); }

    // SIMD 변환
    inline XMMATRIX ToSimd() const { return XMLoadFloat4x4(&m); }

    // 연산자 오버로딩 및 주요 함수 (구현은 cpp에서)
    Matrix operator*(const Matrix& other) const;
    Matrix Transpose() const;
    Matrix Inverse() const;

    // 정적 생성 함수들
    static Matrix Identity();
    static Matrix Translation(float x, float y, float z);
    static Matrix Translation(const Vector3& v);
    static Matrix Scaling(float x, float y, float z);
    static Matrix Scaling(const Vector3& v);
    static Matrix RotationX(float angle);
    static Matrix RotationY(float angle);
    static Matrix RotationZ(float angle);

    // 쿼터니언으로부터 회전 행렬을 만드는 함수 (추후 필요)
    static Matrix RotationQuaternion(const struct Quaternion& q);
};