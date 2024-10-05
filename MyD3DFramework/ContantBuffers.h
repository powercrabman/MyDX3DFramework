#pragma once
//----------------------------
// Struct For Constant Buffer
//----------------------------

struct cbStructSpotLight
{
	Color Ambient;
	Color Diffuse;
	Color Specular;

	Vector3 Position;
	float Range;

	Vector3 SpotDirection;
	float Exponent;

	Vector3 Attenuation;
	float Pad;
};

struct cbStructPointLight
{
	Color Ambient;
	Color Diffuse;
	Color Specular;

	Vector3 Position;
	float Range;

	Vector3 Attenuation;
	float Pad;
};

struct cbStructDirLight
{
	Color Ambient;
	Color Diffuse;
	Color Specular;

	Vector3 Direction;
	float Pad;
};

//----------------------------
// Constant Buffer
//----------------------------

struct cbPerObject
{
	Matrix World;
    Matrix WorldInvTranspose;
    Material Material;
};

struct cbPerFrame
{
	cbStructDirLight	DirLight[DIRECTIONAL_LIGHT_MAXIMUM_COUNT];
	cbStructPointLight	PointLight[POINT_LIGHT_MAXIMUM_COUNT];
	cbStructSpotLight	SpotLight[SPOT_LIGHT_MAXIMUM_COUNT];

	Matrix	ViewProj;
	Vector4 EyePosW;

	unsigned int DirLightCount;    
	unsigned int PointLightCount;  
	unsigned int SpotLightCount;   
	float Pad;
};

struct cbPerRarely
{
	Vector4 FogColor;
	float FogStart;
	float FogRange;
	Vector2 Pad;
};