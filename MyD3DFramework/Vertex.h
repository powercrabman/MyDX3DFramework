#pragma once

struct VertexColor
{
	Vector3 Position	= Vector3::Zero;
	Color	Color{ Colors::White };
};

struct VertexNormal
{
	Vector3 Position	= Vector3::Zero;
	Vector3	Normal		= Vector3::Zero;
};

struct VertexNormalTexture
{
	Vector3 Position = Vector3::Zero;
	Vector3	Normal = Vector3::Zero;
	Vector2	TexCoord = Vector2::Zero;
};
