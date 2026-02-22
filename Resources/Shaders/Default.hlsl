// 상수 버퍼 정의 (C++에서 보낸 데이터가 여기 b0 슬롯으로 들어옵니다)
// HLSL에서 cbuffer 내부에 선언된 변수는 쉐이더 함수 어디서든 그냥 가져다 쓸 수 있는 전역 변수처럼 동작
cbuffer TransformData : register(b0)
{
    float3 offset; // 위치 이동값
    float padding; // 16바이트 정렬을 위한 패딩
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION; // Vertex Shader의 위치 출력은 무조건 SV_POSITION
    float4 color : COLOR;
};

// 버텍스 셰이더: 정점의 위치를 잡아줍니다.
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    // CPU에서 받은 offset을 더해줍니다.
    
    float3 finalPos = input.pos + offset;
    output.pos = float4(finalPos, 1.0f);
    output.color = input.color;
    return output;
}

// 2. 픽셀 셰이더: 화면에 색을 칠합니다.
// 픽셀 쉐이더는 버텍스 쉐이더에서 보간된 데이터를 받으므로 SV_POSITION or TEXCOORD 사용
float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
}