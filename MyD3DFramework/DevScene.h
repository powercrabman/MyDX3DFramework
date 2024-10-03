#pragma once
#include "Scene.h"

class DevScene : public Scene
{
public:
	DevScene() { /* 씬의 생성자에선 객체의 생성을 담당하지 않음 */ }
	virtual ~DevScene() {}

	void EnterScene() override;
	void UpdateScene() override;
	void ExitScene() override;
};
