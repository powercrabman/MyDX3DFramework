#include "EngineConfig.h"

// ---------------------------------------
//  ����ü
// ---------------------------------------

struct Material
{
    float4 Ambient;
    float4 Diffuse; 
    float4 Specular; // W ��Ҵ� �ݻ籤 ����Ʈ �ڻ��� ��Ģ ����
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
//  ������Ʈ ����
// ---------------------------------------

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

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

cbuffer cbPerRarely : register(b2)
{
    float4 gFogColor;
    float gFogStart;
    float gFogRange;
}

// ---------------------------------------
//  ���� ��ȯ�� �����ϴ� ���ؽ� ���̴�
// ---------------------------------------
struct SIMPLE_VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

// ---------------------------------------
//  ���� ��ȯ�� �������� ����� ���ؽ� ���̴�
// ---------------------------------------

struct LIGHT_VS_INPUT
{
    //���� ��ǥ
    float3 PosL : POSITION;

    //���� ��������
    float3 NormalL : NORMAL;
};

struct LIGHT_VS_OUTPUT
{
    //���� ��ǥ�� ��ǥ
    float4 PosH : SV_POSITION;

    //���� ��ǥ�� ��ǥ(���� ����� ����)
    float3 PosW : POSITION;
    
    //���� ��ǥ�� ��������(���� ����� ����)
    float3 NormalW : NORMAL;
};

// ---------------------------------------
//  ���� ��ȯ + ���� + �ؽ�ó
// ---------------------------------------

struct TEX_VS_INPUT
{
    //���� ��ǥ
    float3 PosL : POSITION;

    //���� ��������
    float3 NormalL : NORMAL;
    
    //�ؽ�ó ��ǥ
    float2 UV : TEXCOORD0;
};

struct TEX_VS_OUTPUT
{
    //���� ��ǥ�� ��ǥ
    float4 PosH : SV_POSITION;

    //���� ��ǥ�� ��ǥ(���� ����� ����)
    float3 PosW : POSITION;
    
    //���� ��ǥ�� ��������(���� ����� ����)
    float3 NormalW : NORMAL;

    //�ؽ�ó ��ǥ
    float2 UV : TEXCOORD0;
};
