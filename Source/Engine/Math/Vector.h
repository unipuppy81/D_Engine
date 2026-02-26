#pragma once
#include "MathHelper.h"

using namespace DirectX;

// --- Vector3 ---
struct Vector3 {
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3(FXMVECTOR v) { XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), v); }

    // 연산자 오버로딩
    Vector3 operator+(const Vector3& other) const { return Vector3(XMVectorAdd(ToSimd(), other.ToSimd())); }
    Vector3 operator-(const Vector3& other) const { return Vector3(XMVectorSubtract(ToSimd(), other.ToSimd())); }
    Vector3 operator*(float scalar) const { return Vector3(XMVectorScale(ToSimd(), scalar)); }
    Vector3 operator*(const Vector3& other) const { return Vector3(XMVectorMultiply(ToSimd(), other.ToSimd())); }

    // SIMD 변환
    inline XMVECTOR ToSimd() const { return XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(this)); }


    // 수학 함수
    float Length() const { return XMVectorGetX(XMVector3Length(ToSimd())); }
    Vector3 Normalize() const { return Vector3(XMVector3Normalize(ToSimd())); }
    float Dot(const Vector3& other) const { return XMVectorGetX(XMVector3Dot(ToSimd(), other.ToSimd())); }
    Vector3 Cross(const Vector3& other) const { return Vector3(XMVector3Cross(ToSimd(), other.ToSimd())); }
};



// --- Vector4 ---
struct Vector4 {
    float x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    Vector4(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(1.0f) {} 
    Vector4(const Vector3& v, float _w = 1.0f) : x(v.x), y(v.y), z(v.z), w(_w) {}
    Vector4(FXMVECTOR v) { XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), v); }


    // 연산자 오버로딩
    Vector4 operator+(const Vector4& other) const { return Vector4(XMVectorAdd(ToSimd(), other.ToSimd())); }
    Vector4 operator-(const Vector4& other) const { return Vector4(XMVectorSubtract(ToSimd(), other.ToSimd())); }
    Vector4 operator*(float scalar) const { return Vector4(XMVectorScale(ToSimd(), scalar)); }
    Vector4 operator*(const Vector4& other) const { return Vector4(XMVectorMultiply(ToSimd(), other.ToSimd())); }

    // SIMD 변환 
    inline XMVECTOR ToSimd() const { return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(this)); }

};