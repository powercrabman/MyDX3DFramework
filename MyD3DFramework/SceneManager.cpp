#include "pch.h"
#include "SceneManager.h"
#include "DevScene.h"

void SceneManager::InitializeScene()
{
	//Scene ����
	CreateScene<DevScene>();
}
