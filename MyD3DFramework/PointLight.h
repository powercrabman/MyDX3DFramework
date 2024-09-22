#pragma once

struct PointLight
{
	//�ֺ���
	Color Ambient;

	//Ȯ�걤
	Color Diffuse;

	//�ݻ籤
	Color Specular;
	
	//Position
	Vector3 Position;
	float Range;

	//���ʴ�� a0, a1, a2�� �ǹ�
	Vector3 Attenuation;
	float Pad;
};
