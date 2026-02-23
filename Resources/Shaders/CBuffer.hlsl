// 상수 버퍼 정의 (C++에서 보낸 데이터가 여기 b0 슬롯으로 들어옵니다)
cbuffer TransformData : register(b0)
{
    float3 offset; // 위치 이동값
    float padding; // 16바이트 정렬을 위한 패딩
};



// 데이터 구조체 정의
struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION; 
    float4 color : COLOR;
};

