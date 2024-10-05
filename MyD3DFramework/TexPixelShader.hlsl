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
    //방향성 광원 계산
    outAmbient = float4(0.f, 0.f, 0.f, 0.f);
    outDiffuse = float4(0.f, 0.f, 0.f, 0.f);
    outSpecular = float4(0.f, 0.f, 0.f, 0.f);
    
    //Light 벡터
    float3 lightVecW = -gDirLight[inIndex].Direction;

    //ambient
    outAmbient = gMaterial.Ambient * gDirLight[inIndex].Ambient;
    
    //램버트 코사인 법칙을 위한 설정
    float diffuseFactor = dot(lightVecW, inNormal);
    
    //만약 diffuse가 0보다 작을 경우, 반사광 설정을 피해야하기에
    //조건문 설정
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
    //초기화
    outAmbient = float4(0.f, 0.f, 0.f, 0.f);
    outDiffuse = float4(0.f, 0.f, 0.f, 0.f);
    outSpecular = float4(0.f, 0.f, 0.f, 0.f);
    
    //범위를 넘어갈 경우 리턴
    float3 lightVecW = gPointLight[inIndex].Position - inPosition;
    float dist = length(lightVecW);
    if (dist > gPointLight[inIndex].Range)
    {
        return;
    }
    
    //Light 벡터 정규화
    lightVecW /= dist;
    
    //ambient
    outAmbient = gMaterial.Ambient * gPointLight[inIndex].Ambient;
    
    //램버트 코사인 법칙을 위한 설정
    float diffuseFactor = dot(lightVecW, inNormal);
    
    //만약 diffuse가 0보다 작을 경우, 반사광 설정을 피해야하기에
    //조건문 설정
    if (diffuseFactor > 0.f)
    {
        outDiffuse = diffuseFactor * gPointLight[inIndex].Diffuse * gMaterial.Diffuse;

        float3 reflectVec = reflect(-lightVecW, inNormal);
        float specFactor = pow(max(dot(reflectVec, inEyeVec), 0.f), gMaterial.Specular.w);
        outSpecular = specFactor * gMaterial.Specular * gPointLight[inIndex].Specular;
    }
    
    //감쇠 계수
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
    //초기화l
    outAmbient = float4(0.f, 0.f, 0.f, 0.f);
    outDiffuse = float4(0.f, 0.f, 0.f, 0.f);
    outSpecular = float4(0.f, 0.f, 0.f, 0.f);
    
    //범위를 넘어갈 경우 리턴
    float3 lightVecW = gSpotLight[inIndex].Position - inPosition;
    float dist = length(lightVecW);
    if (dist > gSpotLight[inIndex].Range)
    {
        return;
    }
    
    //Light 벡터 정규화
    lightVecW /= dist;
    
    //ambient
    outAmbient = gMaterial.Ambient * gSpotLight[inIndex].Ambient;
    
    //램버트 코사인 법칙을 위한 설정
    float diffuseFactor = dot(lightVecW, inNormal);
    
    //만약 diffuse가 0보다 작을 경우, 반사광 설정을 피해야하기에
    //조건문 설정
    if (diffuseFactor > 0.f)
    {
        outDiffuse = diffuseFactor * gSpotLight[inIndex].Diffuse * gMaterial.Diffuse;

        float3 reflectVec = reflect(-lightVecW, inNormal);
        float specFactor = pow(max(dot(reflectVec, inEyeVec), 0.f), gMaterial.Specular.w);
        outSpecular = specFactor * gMaterial.Specular * gSpotLight[inIndex].Specular;
    }
    
    //스포트라이트 계수
    float spotFactor = pow(max(0, dot(-lightVecW, gSpotLight[inIndex].SpotDirection)), gSpotLight[inIndex].
    Exponent);
    
    //스포트라이트 계수 * 감쇠 계수
    float att = dot(gSpotLight[inIndex].Attenuation, float3(1.f, dist, dist * dist));
    att = max(att, SMALL_NUMBER);
    
    float attInv = spotFactor / att;
   
    outAmbient *= spotFactor;
    outSpecular *= attInv;
    outDiffuse *= attInv;
}

float4 PS(TEX_VS_OUTPUT input) : SV_TARGET
{
    //텍스처 매핑 미리 수행
    float4 texColor = txDiffuse.Sample(samLinear, input.UV);
    clip(texColor.a - 0.1f); //텍스처의 알파 값이 0.1 미만이면 클리핑
    
    //시선 벡터
    float3 eyeVecW = gEyePosW.xyz - input.PosW;
    float distToEye = length(eyeVecW);
    eyeVecW /= distToEye;

    //초기화
    float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
    float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
    float4 specular = float4(0.f, 0.f, 0.f, 0.f);

    //함수의 매개변수를 받아주는 변수
    float4 a = float4(0.f, 0.f, 0.f, 0.f);
    float4 d = float4(0.f, 0.f, 0.f, 0.f);
    float4 s = float4(0.f, 0.f, 0.f, 0.f);
    
    int i;
    
    //방향성 광원
    for (i = 0; i < gDirLightCount; ++i)
    {
        ComputeDirectionalLight(i, input.NormalW, eyeVecW, a, d, s);
        ambient += a;
        specular += s;
        diffuse += d;
    }

    //점 광원
    for (i = 0; i < gPointLightCount; ++i)
    {
        ComputePointLight(i, input.NormalW, eyeVecW, input.PosW, a, d, s);
        ambient += a;
        specular += s;
        diffuse += d;
    }

    
    //스포트라이트
    for (i = 0; i < gSpotLightCount; ++i)
    {
        ComputeSpotLight(i, input.NormalW, eyeVecW, input.PosW, a, d, s);
        ambient += a;
        specular += s;
        diffuse += d;
    }

    //Light
    float4 fianlColor = (ambient + diffuse) * texColor + specular;
    
    //Fog
    float fogLerp = saturate((distToEye - gFogStart) / gFogRange);
    fianlColor = lerp(fianlColor, gFogColor, fogLerp);
    
    //Texture + Material Alpha
    fianlColor.w = gMaterial.Diffuse.w * texColor.a; //알파 값은 머테리얼에서 가져옴
    return fianlColor;
}



