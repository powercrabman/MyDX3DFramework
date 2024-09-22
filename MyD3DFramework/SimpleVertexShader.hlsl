#include "ShaderHeader.hlsli"

SIMPLE_VS_OUTPUT VS(
    float4 Pos : POSITION,
    float4 Color : COLOR)
{
    SIMPLE_VS_OUTPUT output;
    output.Pos = mul(Pos, gWorld);
    output.Pos = mul(output.Pos, gViewProj);
    output.Color = Color;
    return output;
}
