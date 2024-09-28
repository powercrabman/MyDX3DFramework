#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

void Scene::EnterSceneCore()
{
	EnterScene();
}

void Scene::UpdateSceneCore(float inDeltaTime)
{

	for (size_t i = 0; i < m_validItemSize; ++i)
	{
		GameObject* obj = m_updateObjRepo[i];
		obj->Update(inDeltaTime);
	}

	UpdateScene(inDeltaTime);
}

void Scene::ExitSceneCore()
{
	ExitScene();
}
