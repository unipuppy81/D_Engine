#include "CBuffer.hlsl"

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
}