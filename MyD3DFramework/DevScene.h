#pragma once
#include "Scene.h"

class DevScene : public Scene
{
public:
	DevScene() {}
	virtual ~DevScene() {}

	void EnterScene() override;
	void UpdateScene(float inDeltaTime) override;
	void ExitScene() override;

};

