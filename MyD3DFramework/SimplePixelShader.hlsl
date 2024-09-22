#include "ShaderHeader.hlsli"

float4 PS(SIMPLE_VS_OUTPUT input) : SV_Target
{
    return input.Color;
}
