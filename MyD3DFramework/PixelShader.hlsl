#include "ShaderHeader.hlsli"

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.Color;
}
