#include "ShaderHeader.hlsli"

void ComputeDirectionalLight(
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
    float3 lightVecW = -gDirLight.Direction;

    //ambient
    outAmbient = gMaterial.Ambient * gDirLight.Ambient;
    
    //램버트 코사인 법칙을 위한 설정
    float diffuseFactor = dot(lightVecW, inNormal);
    
    //만약 diffuse가 0보다 작을 경우, 반사광 설정을 피해야하기에
    //조건문 설정
    if (diffuseFactor > 0.f)
    {
        outDiffuse = diffuseFactor * gDirLight.Diffuse * gMaterial.Diffuse;

        float3 reflectVec = reflect(-lightVecW, inNormal);
        float specFactor = pow(max(dot(reflectVec, inEyeVec), 0.f), gMaterial.Specular.w);
        outSpecular = specFactor * gMaterial.Specular * gDirLight.Specular;
    }
}

void ComputePointLight(
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
    float3 lightVecW = gPointLight.Position - inPosition;
    float dist = length(lightVecW);
    if (dist > gPointLight.Range)
    {
        return;
    }
    
    //Light 벡터 정규화
    lightVecW /= dist;
    
    //ambient
    outAmbient = gMaterial.Ambient * gPointLight.Ambient;
    
    //램버트 코사인 법칙을 위한 설정
    float diffuseFactor = dot(lightVecW, inNormal);
    
    //만약 diffuse가 0보다 작을 경우, 반사광 설정을 피해야하기에
    //조건문 설정
    if (diffuseFactor > 0.f)
    {
        outDiffuse = diffuseFactor * gPointLight.Diffuse * gMaterial.Diffuse;

        float3 reflectVec = reflect(-lightVecW, inNormal);
        float specFactor = pow(max(dot(reflectVec, inEyeVec), 0.f), gMaterial.Specular.w);
        outSpecular = specFactor * gMaterial.Specular * gPointLight.Specular;
    }
    
    //감쇠 계수
    float att = dot(gPointLight.Attenuation, float3(1.f, dist, dist * dist));
    att = max(att, SMALL_NUMBER);
    
    float attInv = 1.f / att;
    
    outSpecular *= attInv;
    outDiffuse *= attInv;
}

void ComputeSpotLight(
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
    float3 lightVecW = gSpotLight.Position - inPosition;
    float dist = length(lightVecW);
    if (dist > gSpotLight.Range)
    {
        return;
    }
    
    //Light 벡터 정규화
    lightVecW /= dist;
    
    //ambient
    outAmbient = gMaterial.Ambient * gSpotLight.Ambient;
    
    //램버트 코사인 법칙을 위한 설정
    float diffuseFactor = dot(lightVecW, inNormal);
    
    //만약 diffuse가 0보다 작을 경우, 반사광 설정을 피해야하기에
    //조건문 설정
    if (diffuseFactor > 0.f)
    {
        outDiffuse = diffuseFactor * gSpotLight.Diffuse * gMaterial.Diffuse;

        float3 reflectVec = reflect(-lightVecW, inNormal);
        float specFactor = pow(max(dot(reflectVec, inEyeVec), 0.f), gMaterial.Specular.w);
        outSpecular = specFactor * gMaterial.Specular * gSpotLight.Specular;
    }
    
    //스포트라이트 계수
    float spotFactor = pow(max(0, dot(-lightVecW, gSpotLight.SpotDirection)), gSpotLight.Exponent);
    
    //스포트라이트 계수 * 감쇠 계수
    float att = dot(gSpotLight.Attenuation, float3(1.f, dist, dist * dist));
    att = max(att, SMALL_NUMBER);
    
    float attInv = spotFactor / att;
   
    outAmbient *= spotFactor;
    outSpecular *= attInv;
    outDiffuse *= attInv;
}

float4 PS(LIGHT_VS_OUTPUT input) : SV_TARGET
{
    //시선 벡터
    float3 eyeVecW = normalize(gEyePosW - input.PosW);

    //초기화
    float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
    float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
    float4 specular = float4(0.f, 0.f, 0.f, 0.f);

    //함수의 매개변수를 받아주는 변수
    float4 a = float4(0.f, 0.f, 0.f, 0.f);
    float4 d = float4(0.f, 0.f, 0.f, 0.f);
    float4 s = float4(0.f, 0.f, 0.f, 0.f);

    //방향성 광원
    ComputeDirectionalLight(input.NormalW, eyeVecW, a, d, s);
    ambient += a;
    specular += s;
    diffuse += d;
    
    //점 광원
    ComputePointLight(input.NormalW, eyeVecW, input.PosW, a, d, s);
    ambient += a;
    specular += s;
    diffuse += d;
    
    //스포트라이트
    ComputeSpotLight(input.NormalW, eyeVecW, input.PosW, a, d, s);
    ambient += a;
    specular += s;
    diffuse += d;
    
    //최종 조명 계산
    float4 finalColor = ambient + diffuse + specular;
    finalColor.w = gMaterial.Diffuse.w; //알파 값은 머테리얼에서 가져옴
    return finalColor;
}