#pragma once

struct PointLight
{
	//ÁÖº¯±¤
	Color Ambient;

	//È®»ê±¤
	Color Diffuse;

	//¹Ý»ç±¤
	Color Specular;
	
	//(X,Y,Z) = Position, (W) = Range
	Vector4 PositionRange;

	//Â÷·Ê´ë·Î a0, a1, a2¸¦ ÀÇ¹Ì, W = ÆÐÅ·
	Vector4 Attenuation;
};



