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
    
cbuffer cPerObject : register(b0)
{
    matrix gWorld;
    matrix gWorldInvTranspose;
    matrix gViewProj;
    Material gMaterial;
};

cbuffer cbPerFrame : register(b1)
{
    DirectionalLight gDirLight;
    PointLight gPointLight;
    SpotLight gSpotLight;
    float3 gEyePosW;
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
    float4 PosL : POSITION;

    //���� ��������
    float4 NormalL : NORMAL;
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
