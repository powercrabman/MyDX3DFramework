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
    DirectionalLight DirLight;
    PointLight PointLight;
    SpotLight SpotLight;
    Vector4 EyePosW;
};