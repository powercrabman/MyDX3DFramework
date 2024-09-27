#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

void Scene::EnterSceneCore()
{
	EnterScene();
}

void Scene::UpdateSceneCore(float inDeltaTime)
{
	for (GameObject* obj : m_updateObjRepo)
	{
		if (obj)
		{
			obj->UpdateCore(inDeltaTime);
		}
	}

	UpdateScene(inDeltaTime);
}

void Scene::ExitSceneCore()
{
	ExitScene();
}
