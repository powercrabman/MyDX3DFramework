#pragma once
#include "Scene.h"

class DevScene : public Scene
{
	// Scene을(를) 통해 상속됨
	void EnterScene() override;
	void UpdateScene(float inDeltaTime) override;
	void ExitScene() override;
};

