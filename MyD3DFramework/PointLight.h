#pragma once

struct PointLight
{
	//ÁÖº¯±¤
	Color Ambient;

	//È®»ê±¤
	Color Diffuse;

	//¹Ý»ç±¤
	Color Specular;
	
	//Position
	Vector3 Position;
	float Range;

	//Â÷·Ê´ë·Î a0, a1, a2¸¦ ÀÇ¹Ì
	Vector3 Attenuation;
	float Pad;
};
