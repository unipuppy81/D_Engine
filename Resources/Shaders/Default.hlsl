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

// 1. 버텍스 셰이더: 점의 위치를 잡아줍니다.
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.color = input.color;
    return output;
}

// 2. 픽셀 셰이더: 화면에 색을 칠합니다.
// 픽셀 쉐이더는 버텍스 쉐이더에서 보간된 데이터를 받으므로 SV_POSITION or TEXCOORD 사용
float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
}