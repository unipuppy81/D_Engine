#include "CBuffer.hlsl"

// 버텍스 셰이더: 정점의 위치를 잡아줍니다.
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    // 위치 변환 (Local -> World -> Projection)
    float4 pos = float4(input.pos, 1.0f);
    
    pos = mul(pos, matWorld);
    pos = mul(pos, matView);
    pos = mul(pos, matProjection);
    
    output.pos = pos;
    output.color = input.color;
    return output;
}
