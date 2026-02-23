#pragma once
#include <DirectXMath.h>

using namespace DirectX;

using float2 = XMFLOAT2;
using float3 = XMFLOAT3;
using float4 = XMFLOAT4;
using Color = XMFLOAT4;



struct Vertex {	// Color
	float3 pos;
	float4 color;
};

struct VertexTexture { // Texture
	float3 pos;
	float2 uv;
};

struct VertexFull { // Lighting + Texture
	float3 pos;
	float2 uv;
	float3 normal;
};