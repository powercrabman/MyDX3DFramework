#pragma once

struct SpotLight
{
	//주변광
	Color Ambient;

	//확산광
	Color Diffuse;

	//반사광
	Color Specular;

	//(X,Y,Z) = Position, (W) = Range
	Vector3 Position;
	float Range;

	//(X,Y,Z) = Direction, (W) = 스포트라이트 램버트 코사인 법칙 지수
	Vector3 SpotDirection;
	float Exponent;

	//차례대로 a0, a1, a2를 의미
	Vector3 Attenuation;
	float Pad;
};
