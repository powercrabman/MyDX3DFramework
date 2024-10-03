#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

void Scene::EnterSceneCore()
{
	EnterScene();
}

void Scene::UpdateSceneCore()
{

	for (size_t i = 0; i < m_validObjSizeinVector; ++i)
	{
		GameObject* obj = m_updateObjRepo[i];
		obj->Update();
	}

	UpdateScene();
}

void Scene::ExitSceneCore()
{
	ExitScene();
}
