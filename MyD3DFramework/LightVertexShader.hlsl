#include "ShaderHeader.hlsli"

LIGHT_VS_OUTPUT VS(LIGHT_VS_INPUT input)
{
    LIGHT_VS_OUTPUT result;
    
    result.PosW = mul(input.PosL, gWorld);
    
    //법선벡터 변환 및 정규화
    result.NormalW = normalize(mul(input.NormalL, gWorldInvTranspose));

    result.PosH = mul(float4(result.PosW, 1.f), gViewProj);
	
    return result;
}