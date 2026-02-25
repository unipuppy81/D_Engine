#pragma once
#include <DirectXMath.h>

using namespace DirectX;

using float2 = XMFLOAT2;
using float3 = XMFLOAT3;
using float4 = XMFLOAT4;
using Color = XMFLOAT4;


// 정점 구조체 (IA 단계용)
struct Vertex {	// Color
	float3 pos;
	float4 color;
	float3 normal;
};

struct VertexTexture { // Texture
	float3 pos;
	float2 uv;
};


// 상수 버퍼 구조체 (VS 단계용)
struct TransformData {
	XMMATRIX matWorld;      // 월드 변환
	XMMATRIX matView;       // 뷰포트 변환
	XMMATRIX matProjection; // 투영 변환 
};


struct LightData {
	float4 diffuse;	// 빛의 색상 (RGBA)
	float3 direction;	// 빛이 나가는 방향
	float padding;		// 패딩
};