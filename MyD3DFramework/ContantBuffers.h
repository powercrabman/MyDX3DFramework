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
    Matrix ViewProj;
    Material Material;
};

struct cbPerFrame
{
	cbStructDirLight DirLight;
	cbStructPointLight PointLight;
	cbStructSpotLight SpotLight;
    Vector4 EyePosW;
};