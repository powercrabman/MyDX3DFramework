#include "ShaderHeader.hlsli"

VS_OUTPUT VS(
    float4 Pos : POSITION,
    float4 Color : COLOR)
{
    VS_OUTPUT output;
    output.Pos = mul(Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Color = Color;
    return output;
}
