#pragma once

struct DirectionalLight
{
	//¡÷∫Ø±§
	Color Ambient;

	//»ÆªÍ±§
	Color Diffuse;

	//π›ªÁ±§
	Color Specular;

	//(X,Y,Z) = Direction
	Vector3 Direction;
	float Pad;
};


