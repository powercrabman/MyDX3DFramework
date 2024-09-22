#pragma once
struct cbPerObject
{
	Matrix World;
    Matrix WorldInvTranspose;
    Matrix ViewProj;
    Material Material;
};

struct cbPerFrame
{
    DirectionalLight gDirLight;
    PointLight gPointLight;
    SpotLight gSpotLight;
    Vector4 gEyePosW;
};