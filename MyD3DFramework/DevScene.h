#pragma once
#include "Scene.h"

class DevScene : public Scene
{
public:
	DevScene() { /* ���� �����ڿ��� ��ü�� ������ ������� ���� */ }
	virtual ~DevScene() {}

	void EnterScene() override;
	void UpdateScene() override;
	void ExitScene() override;
};
