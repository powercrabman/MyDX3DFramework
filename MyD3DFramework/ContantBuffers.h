#pragma once
struct cbPerObject
{
	Matrix World;
    Matrix WorldInvTranspose;
    Matrix ViewProj;
};

struct cbPerFrame
{
    DirectionalLight gDirLight;
    PointLight gPointLight;
    SpotLight gSpotLight;
    Vector4 gEyePosW;
};