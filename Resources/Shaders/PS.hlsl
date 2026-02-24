#include "CBuffer.hlsl"

float4 PS(VS_OUTPUT input) : SV_Target
{

    // 1. 노멀 및 광원 방향 정규화
    float3 N = normalize(input.normal);
    float3 L = normalize(-lightDir.xyz); // xyz만 명시적으로 사용
    
    // 2. Diffuse 강도 (0.0 ~ 1.0)
    float intensity = max(dot(N, L), 0.0f);
    
    // 3. 최종 색상 계산
    float4 diffuse = input.color * lightDiffuse * intensity;
    
    // Ambient: (물체색 * 0.2f) -> 이렇게 해야 빛 안받는 곳도 무지개색이 유지됨!
    float4 ambient = input.color * 0.2f;
    
    float4 finalColor = diffuse + ambient;
    finalColor.a = 1.0f;
    
    return finalColor;
}