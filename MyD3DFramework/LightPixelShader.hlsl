#include "ShaderHeader.hlsli"

void ComputeDirectionalLight(
    in int inIndex,
    in float3 inNormal,
    in float3 inEyeVec,
    out float4 outAmbient,
    out float4 outDiffuse,
    out float4 outSpecular
    )
{
    //���⼺ ���� ���
    outAmbient = float4(0.f, 0.f, 0.f, 0.f);
    outDiffuse = float4(0.f, 0.f, 0.f, 0.f);
    outSpecular = float4(0.f, 0.f, 0.f, 0.f);
    
    //Light ����
    float3 lightVecW = -gDirLight[inIndex].Direction;

    //ambient
    outAmbient = gMaterial.Ambient * gDirLight[inIndex].Ambient;
    
    //����Ʈ �ڻ��� ��Ģ�� ���� ����
    float diffuseFactor = dot(lightVecW, inNormal);
    
    //���� diffuse�� 0���� ���� ���, �ݻ籤 ������ ���ؾ��ϱ⿡
    //���ǹ� ����
    if (diffuseFactor > 0.f)
    {
        outDiffuse = diffuseFactor * gDirLight[inIndex].Diffuse * gMaterial.Diffuse;

        float3 reflectVec = reflect(-lightVecW, inNormal);
        float specFactor = pow(max(dot(reflectVec, inEyeVec), 0.f), gMaterial.Specular.w);
        outSpecular = specFactor * gMaterial.Specular * gDirLight[inIndex].Specular;
    }
}

void ComputePointLight(
    in int inIndex,
    in float3 inNormal,
    in float3 inEyeVec,
    in float3 inPosition,
    out float4 outAmbient,
    out float4 outDiffuse,
    out float4 outSpecular
    )
{
    //�ʱ�ȭ
    outAmbient = float4(0.f, 0.f, 0.f, 0.f);
    outDiffuse = float4(0.f, 0.f, 0.f, 0.f);
    outSpecular = float4(0.f, 0.f, 0.f, 0.f);
    
    //������ �Ѿ ��� ����
    float3 lightVecW = gPointLight[inIndex].Position - inPosition;
    float dist = length(lightVecW);
    if (dist > gPointLight[inIndex].Range)
    {
        return;
    }
    
    //Light ���� ����ȭ
    lightVecW /= dist;
    
    //ambient
    outAmbient = gMaterial.Ambient * gPointLight[inIndex].Ambient;
    
    //����Ʈ �ڻ��� ��Ģ�� ���� ����
    float diffuseFactor = dot(lightVecW, inNormal);
    
    //���� diffuse�� 0���� ���� ���, �ݻ籤 ������ ���ؾ��ϱ⿡
    //���ǹ� ����
    if (diffuseFactor > 0.f)
    {
        outDiffuse = diffuseFactor * gPointLight[inIndex].Diffuse * gMaterial.Diffuse;

        float3 reflectVec = reflect(-lightVecW, inNormal);
        float specFactor = pow(max(dot(reflectVec, inEyeVec), 0.f), gMaterial.Specular.w);
        outSpecular = specFactor * gMaterial.Specular * gPointLight[inIndex].Specular;
    }
    
    //���� ���
    float att = dot(gPointLight[inIndex].Attenuation, float3(1.f, dist, dist * dist));
    att = max(att, SMALL_NUMBER);
    
    float attInv = 1.f / att;
    
    outSpecular *= attInv;
    outDiffuse *= attInv;
}

void ComputeSpotLight(
    in int inIndex,
    in float3 inNormal,
    in float3 inEyeVec,
    in float3 inPosition,
    out float4 outAmbient,
    out float4 outDiffuse,
    out float4 outSpecular
    )
{
    //�ʱ�ȭl
    outAmbient = float4(0.f, 0.f, 0.f, 0.f);
    outDiffuse = float4(0.f, 0.f, 0.f, 0.f);
    outSpecular = float4(0.f, 0.f, 0.f, 0.f);
    
    //������ �Ѿ ��� ����
    float3 lightVecW = gSpotLight[inIndex].Position - inPosition;
    float dist = length(lightVecW);
    if (dist > gSpotLight[inIndex].Range)
    {
        return;
    }
    
    //Light ���� ����ȭ
    lightVecW /= dist;
    
    //ambient
    outAmbient = gMaterial.Ambient * gSpotLight[inIndex].Ambient;
    
    //����Ʈ �ڻ��� ��Ģ�� ���� ����
    float diffuseFactor = dot(lightVecW, inNormal);
    
    //���� diffuse�� 0���� ���� ���, �ݻ籤 ������ ���ؾ��ϱ⿡
    //���ǹ� ����
    if (diffuseFactor > 0.f)
    {
        outDiffuse = diffuseFactor * gSpotLight[inIndex].Diffuse * gMaterial.Diffuse;

        float3 reflectVec = reflect(-lightVecW, inNormal);
        float specFactor = pow(max(dot(reflectVec, inEyeVec), 0.f), gMaterial.Specular.w);
        outSpecular = specFactor * gMaterial.Specular * gSpotLight[inIndex].Specular;
    }
    
    //����Ʈ����Ʈ ���
    float spotFactor = pow(max(0, dot(-lightVecW, gSpotLight[inIndex].SpotDirection)), gSpotLight[inIndex].
    Exponent);
    
    //����Ʈ����Ʈ ��� * ���� ���
    float att = dot(gSpotLight[inIndex].Attenuation, float3(1.f, dist, dist * dist));
    att = max(att, SMALL_NUMBER);
    
    float attInv = spotFactor / att;
   
    outAmbient *= spotFactor;
    outSpecular *= attInv;
    outDiffuse *= attInv;
}

float4 PS(LIGHT_VS_OUTPUT input) : SV_TARGET
{
    //�ü� ����
    float3 eyeVecW = normalize(gEyePosW.xyz - input.PosW);

    //�ʱ�ȭ
    float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
    float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
    float4 specular = float4(0.f, 0.f, 0.f, 0.f);

    //�Լ��� �Ű������� �޾��ִ� ����
    float4 a = float4(0.f, 0.f, 0.f, 0.f);
    float4 d = float4(0.f, 0.f, 0.f, 0.f);
    float4 s = float4(0.f, 0.f, 0.f, 0.f);
    
    int i;

    //���⼺ ����
    for (i = 0; i < gDirLightCount;++i)
    {
        ComputeDirectionalLight(i, input.NormalW, eyeVecW, a, d, s);
        ambient += a;
        specular += s;
        diffuse += d;
    }

    
    //�� ����
    for (i = 0; i < gPointLightCount; ++i)
    {
        ComputePointLight(i, input.NormalW, eyeVecW, input.PosW, a, d, s);
        ambient += a;
        specular += s;
        diffuse += d;
    }

    
    //����Ʈ����Ʈ
    for (i = 0; i < gSpotLightCount; ++i)
    {
        ComputeSpotLight(i, input.NormalW, eyeVecW, input.PosW, a, d, s);
        ambient += a;
        specular += s;
        diffuse += d;
    }

    
    //���� ���� ���
    float4 finalColor = ambient + diffuse + specular;
    finalColor.w = gMaterial.Diffuse.w; //���� ���� ���׸��󿡼� ������
    return finalColor;
}