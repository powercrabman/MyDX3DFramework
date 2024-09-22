#pragma once

struct SpotLight
{
	//�ֺ���
	Color Ambient;

	//Ȯ�걤
	Color Diffuse;

	//�ݻ籤
	Color Specular;

	//(X,Y,Z) = Position, (W) = Range
	Vector3 Position;
	float Range;

	//(X,Y,Z) = Direction, (W) = ����Ʈ����Ʈ ����Ʈ �ڻ��� ��Ģ ����
	Vector3 SpotDirection;
	float Exponent;

	//���ʴ�� a0, a1, a2�� �ǹ�
	Vector3 Attenuation;
	float Pad;
};
