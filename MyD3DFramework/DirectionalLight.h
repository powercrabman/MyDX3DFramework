#pragma once

struct DirectionalLight
{
	//�ֺ���
	Color Ambient;

	//Ȯ�걤
	Color Diffuse;

	//�ݻ籤
	Color Specular;

	//(X,Y,Z) = Direction
	Vector3 Direction;
	float Pad;
};


