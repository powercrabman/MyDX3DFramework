#include "ShaderHeader.hlsli"

TEX_VS_OUTPUT VS(TEX_VS_INPUT input)
{
    TEX_VS_OUTPUT result;
    
    result.PosW = mul(float4(input.PosL, 1.0f), gWorld);
    
    result.NormalW = normalize(mul(float4(input.NormalL, 0.0f), gWorldInvTranspose));

    result.PosH = mul(float4(result.PosW, 1.0f), gViewProj);
    
    result.UV = input.UV;
	
    return result;
}