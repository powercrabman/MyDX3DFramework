#pragma once
#include "Scene.h"

class SceneManager final
{
	DEFINE_SINGLE(SceneManager);
public:
	~SceneManager() = default;

	void InitializeScene();
	Scene* GetCurrentScene() const { return m_curScene; }

	/* �� ���� */
	template<typename SceneType>
	void SwitchScene();

private:
	SceneManager();
	
	/* �� ���� */
	template<typename SceneType>
	Scene* CreateScene();

private:
	constexpr static size_t sReserveCapacity = 32;
	std::unordered_map<CM::TypeID, std::unique_ptr<Scene>> m_sceneRepo;
	Scene* m_curScene = nullptr;
};


SceneManager::SceneManager()
{
	m_sceneRepo.reserve(sReserveCapacity);
}

template<typename SceneType>
Scene* SceneManager::CreateScene()
{
	static_assert(std::is_base_of<Scene, SceneType>::value, "SceneType is not base of Scene.");
	std::unique_ptr<SceneType> sc = std::make_unique<SceneType>();
	SceneType* ptr = sc.get();

	auto iter = m_sceneRepo.find(CM::TypeTrait<SceneType>::ID());

	assert(iter == m_sceneRepo.end());

#ifndef _DEBUG
	if (iter != m_sceneRepo.end())
	{
		return iter->second.get();
	}
#endif // !_DEBUG

	m_sceneRepo[CM::TypeTrait<SceneType>::ID()] = std::move(sc);
	return ptr;
}

template<typename SceneType>
void SceneManager::SwitchScene()
{
	auto iter = m_sceneRepo.find(CM::TypeTrait<SceneType>::ID());
	assert(iter != m_sceneRepo.end());

	//���� Scene ó��
	m_curScene->ExitSceneCore();

	//Scene ����
	Scene* sceneToSwitch = iter->second.get();
	m_curScene = sceneToSwitch;
	m_curScene->EnterSceneCore();
}
