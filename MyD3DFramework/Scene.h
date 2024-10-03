#pragma once

#include "GameObject.h"

class Scene
{
	class GameObjectStorage;
public:
	virtual ~Scene() = default;

	//씬 메인 함수
	void EnterSceneCore();
	void UpdateSceneCore();
	void ExitSceneCore();

	/* 오브젝트 생성 */
	template <typename ObjType, typename... Args>
	ObjType* CreateGameObject(Args&&...args);

	template <typename ObjType, typename... Args>
	ObjType* CreateNamedGameObject(std::string_view inObjectName, Args&&...args);

	/* 탐색 */
	GameObject* GetGameObjectOrNull(const CM::Name& inObjName);

	/* 탐색2 */
	GameObject* FindGameObjectOrNull(const std::function<bool(const GameObject*)>& inFindFunction);

	/* 삭제 */
	void RemoveGameObject(const CM::Name inObjName);
	void RemoveGameObject(GameObject* inObject);

	/* 가비지 컬렉터 */
	void CleanGarbge();
	void ClearRepogitory();

protected:
	Scene();

	virtual void EnterScene() = 0;
	virtual void UpdateScene() = 0;
	virtual void ExitScene() = 0;

private:
	void AddObjectToRepositories(std::unique_ptr<GameObject> inObject);

private:
	constexpr static size_t sReserveCapacity = 1024;

	std::unordered_map<CM::Name, std::pair<size_t, std::unique_ptr<GameObject>>> m_gameObjRepo;
	std::vector<GameObject*> m_updateObjRepo;
	size_t m_validObjSizeinVector = 0;
};

/* 탐색 */
inline GameObject* Scene::GetGameObjectOrNull(const CM::Name& inObjName)
{
	auto iter = m_gameObjRepo.find(inObjName);
	if (iter != m_gameObjRepo.end())
	{
		return iter->second.second.get();
	}
	return nullptr;
}

/* 탐색2 */
inline GameObject* Scene::FindGameObjectOrNull(const std::function<bool(const GameObject*)>& inFindFunction)
{
	for (size_t i = 0; i < m_validObjSizeinVector; ++i)
	{
		GameObject* obj = m_updateObjRepo[i];
		if (inFindFunction(obj))
		{
			return obj;
		}
	}

	return nullptr;
}


/* 삭제 */
inline void Scene::RemoveGameObject(const CM::Name inObjName)
{
	//삭제할 요소 찾기
	auto iter = m_gameObjRepo.find(inObjName);
	assert(iter != m_gameObjRepo.end());

	//배열의 인덱스
	size_t arrayIdx = (*iter).second.first;

	//만약 요소가 1개라면 early return
	if (m_validObjSizeinVector == 1)
	{
		//해쉬 속 기존 ItemA 요소 제거
		m_updateObjRepo[0] = nullptr;
		m_gameObjRepo.erase(inObjName);
		m_validObjSizeinVector = 0;

		return;
	}

	//ItemA와 ItemB를 찾은후 자리 변경, ItemA는 nullptr로 변경
	m_updateObjRepo[arrayIdx] = m_updateObjRepo[m_validObjSizeinVector - 1];
	m_updateObjRepo[m_validObjSizeinVector - 1] = nullptr;
	--m_validObjSizeinVector;

	//해쉬 속 기존 ItemA 요소 제거
	m_gameObjRepo.erase(inObjName);

	//해쉬 속 ItemB의 배열 인덱스 업데이트
	const CM::Name& oldName = m_updateObjRepo[arrayIdx]->GetName();
	m_gameObjRepo[oldName].first = arrayIdx;
}

inline void Scene::RemoveGameObject(GameObject* inObject)
{
	RemoveGameObject(inObject->GetName());
}

/* 가비지 컬렉터 */
inline void Scene::CleanGarbge()
{
	if (m_validObjSizeinVector < m_updateObjRepo.size())
	{
		m_updateObjRepo.erase(m_updateObjRepo.begin() + m_validObjSizeinVector, m_updateObjRepo.end());
	}
}

inline void Scene::ClearRepogitory()
{
	m_gameObjRepo.clear();
	m_updateObjRepo.clear();
	m_validObjSizeinVector = 0;
}

inline Scene::Scene()
{
	m_gameObjRepo.reserve(sReserveCapacity);
	m_updateObjRepo.reserve(sReserveCapacity);
}

inline void Scene::AddObjectToRepositories(std::unique_ptr<GameObject> inObject)
{
	//배열에 업데이트
	size_t backIndex = m_updateObjRepo.size();

	assert(backIndex >= m_validObjSizeinVector);

	if (backIndex == m_validObjSizeinVector)
	{
		m_updateObjRepo.push_back(inObject.get());
		++m_validObjSizeinVector;
	}
	else
	{
		m_updateObjRepo[m_validObjSizeinVector++] = inObject.get();
	}

	//해쉬에 업데이트
	const CM::Name& name = inObject->GetName();
	ASSERT(!m_gameObjRepo.contains(name), "동일한 ID를 가진 오브젝트가 씬 내에 이미 존재합니다.");
	m_gameObjRepo[name] = std::make_pair(m_validObjSizeinVector - 1, std::move(inObject));
}

template<typename ObjType, typename ...Args>
inline ObjType* Scene::CreateGameObject(Args && ...args)
{
	static_assert(std::is_base_of<GameObject, ObjType>::value, "ObjType is not derivation of GameObject");
	std::unique_ptr<ObjType> obj = std::make_unique<ObjType>(std::forward<Args>(args)...);
	obj->SetupObject(CM::GetTypeName<ObjType>(), false);
	obj->InitalizeCore();
	ObjType* ptr = obj.get();
	AddObjectToRepositories(std::move(obj));

	return ptr;
}

template<typename ObjType, typename ...Args>
inline ObjType* Scene::CreateNamedGameObject(std::string_view inObjectName, Args && ...args)
{
	static_assert(std::is_base_of<GameObject, ObjType>::value, "ObjType is not derivation of GameObject");
	std::unique_ptr<ObjType> obj = std::make_unique<ObjType>(std::forward<Args>(args)...);
	obj->SetupObject(inObjectName, true);
	obj->InitalizeCore();
	ObjType* ptr = obj.get();
	AddObjectToRepositories(std::move(obj));

	return ptr;
}
