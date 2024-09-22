#pragma once

struct PointLight
{
	//�ֺ���
	Color Ambient;

	//Ȯ�걤
	Color Diffuse;

	//�ݻ籤
	Color Specular;
	
	//(X,Y,Z) = Position, (W) = Range
	Vector4 PositionRange;

	//���ʴ�� a0, a1, a2�� �ǹ�, W = ��ŷ
	Vector4 Attenuation;
};



