#include "EngineConfig.h"

// ---------------------------------------
//  구조체
// ---------------------------------------

struct Material
{
    float4 Ambient;
    float4 Diffuse; 
    float4 Specular; // W 요소는 반사광 램버트 코사인 법칙 지수
};

struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float  Pad;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 Attenuation;
    float Pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 SpotDirection;
    float Exponent;
    float3 Attenuation;
    float Pad;
};

#define SMALL_NUMBER (1.0e-6)
    
// ---------------------------------------
//  컨스턴트 버퍼
// ---------------------------------------
    
cbuffer cPerObject : register(b0)
{
    matrix gWorld;
    matrix gWorldInvTranspose;
    Material gMaterial;
};

cbuffer cbPerFrame : register(b1)
{
    DirectionalLight gDirLight[DIRECTIONAL_LIGHT_MAXIMUM_COUNT];
    PointLight gPointLight[POINT_LIGHT_MAXIMUM_COUNT];
    SpotLight gSpotLight[SPOT_LIGHT_MAXIMUM_COUNT];

    matrix gViewProj;
    float4 gEyePosW;
    
    unsigned int gDirLightCount;
    unsigned int gPointLightCount;
    unsigned int gSpotLightCount;
    float Pad;
}
// ---------------------------------------
//  선형 변환만 정의하는 버텍스 쉐이더
// ---------------------------------------
struct SIMPLE_VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

// ---------------------------------------
//  선형 변환과 광원까지 고려한 버텍스 쉐이더
// ---------------------------------------

struct LIGHT_VS_INPUT
{
    //로컬 좌표
    float3 PosL : POSITION;

    //로컬 법선벡터
    float3 NormalL : NORMAL;
};

struct LIGHT_VS_OUTPUT
{
    //동차 좌표계 좌표
    float4 PosH : SV_POSITION;

    //월드 좌표계 좌표(광원 계산을 위함)
    float3 PosW : POSITION;
    
    //월드 좌표계 법선벡터(광원 계산을 위함)
    float3 NormalW : NORMAL;
};
