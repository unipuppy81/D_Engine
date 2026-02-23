#include "CBuffer.hlsl"

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
