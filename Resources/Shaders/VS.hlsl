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
    
    // 조명 (Normal 변환)
    // 1. float3x3으로 캐스팅해서 방향만 순수하게 곱합니다.
    // 2. mul 결과가 1.0보다 크거나 작을 수 있으므로 반드시 다시 normalize 합니다.
    output.normal = normalize(mul(input.normal, (float3x3) matWorld));
    
    return output;
}
