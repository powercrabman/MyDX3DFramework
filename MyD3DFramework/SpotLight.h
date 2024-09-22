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
	Vector4 PositionRange;

	//(X,Y,Z) = Direction, (W) = ����Ʈ����Ʈ ����Ʈ �ڻ��� ��Ģ ����
	Vector4 DirectionSpot;

	//���ʴ�� a0, a1, a2�� �ǹ�, W = �е�
	Vector4 Attenuation;
};
