#pragma once
#include "Vector.h"

struct Matrix;

struct Quaternion {
    union {
        struct { float x, y, z, w; };
        Vector4 v4;
    };

    Quaternion() : x(0), y(0), z(0), w(1) {}
    Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    Quaternion(FXMVECTOR v);

    inline XMVECTOR ToSimd() const { return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(this)); }

    // 연산자 및 주요 함수
    Quaternion operator*(const Quaternion& other) const;

    // 유틸리티 함수들
    static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
    static Quaternion FromEuler(float pitch, float yaw, float roll);

    // Matrix를 반환하므로 구현은 cpp에서 수행
    Matrix ToMatrix() const;
};