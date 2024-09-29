#pragma once

#include "GameObject.h"

class Scene
{
	class GameObjectStorage;
public:
	virtual ~Scene() = default;

	//씬 메인 함수							
	void EnterSceneCore();
	void UpdateSceneCore(float inDeltaTime);
	void ExitSceneCore();

    template <typename ObjType, typename... Args>
	inline GameObject* CreateGameObject(Args&&...args);

	/* 탐색 */
	inline GameObject* GetGameObjectOrNull(uint64 inID);

	/* 탐색2 */
	inline GameObject* FindGameObjectOrNull(const std::function<bool(const GameObject*)>& inFindFunction);

	/* 삭제 */
	inline void RemoveGameObject(uint64 inID);

	/* 가비지 컬렉터 */
	inline void CleanGarbge();

protected:
	Scene();

	virtual void EnterScene() = 0;
	virtual void UpdateScene(float inDeltaTime) = 0;
	virtual void ExitScene() = 0;
private:
	//CM::TypeInfo m_typeInfo = {};

	using GameObjectRef = std::unique_ptr<GameObject>;
	constexpr inline static size_t sReserveCapacity = 1024;

	std::unordered_map<size_t, std::pair<size_t, GameObjectRef>> m_gameObjRepo;
	std::vector<GameObject*> m_updateObjRepo;
	size_t m_validObjSizeinVector = 0;
};

template<typename ObjType, typename ...Args>
inline GameObject* Scene::CreateGameObject(Args && ...args)
{
	static_assert(std::is_base_of<GameObject, ObjType>::value, "ObjType is not derivation of GameObject");
	GameObjectRef obj = std::make_unique<ObjType>(std::forward<Args>(args)...);
	GameObject* ptr = obj.get();

	//배열에 업데이트
	size_t backIndex = m_updateObjRepo.size();

	assert(backIndex >= m_validObjSizeinVector);

	if (backIndex == m_validObjSizeinVector)
	{
		m_updateObjRepo.push_back(ptr);
		++m_validObjSizeinVector;
	}
	else
	{
		m_updateObjRepo[m_validObjSizeinVector++] = ptr;
	}

	//해쉬에 업데이트
	m_gameObjRepo[ptr->GetObjectID()] = std::make_pair(m_validObjSizeinVector - 1, std::move(obj));

	return ptr;
}

/* 탐색 */
inline GameObject* Scene::GetGameObjectOrNull(uint64 inID)
{
	auto iter = m_gameObjRepo.find(inID);
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
inline void Scene::RemoveGameObject(uint64 inID)
{
	//삭제할 요소 찾기
	auto iter = m_gameObjRepo.find(inID);
	assert(iter != m_gameObjRepo.end());

	//배열의 인덱스
	size_t arrayIdx = (*iter).second.first;

	//만약 요소가 1개라면 early return
	if (m_validObjSizeinVector == 1)
	{
		//해쉬 속 기존 ItemA 요소 제거
		m_updateObjRepo[0] = nullptr;
		m_gameObjRepo.erase(inID);
		m_validObjSizeinVector = 0;

		return;
	}

	//ItemA와 ItemB를 찾은후 자리 변경, ItemA는 nullptr로 변경
	m_updateObjRepo[arrayIdx] = m_updateObjRepo[m_validObjSizeinVector - 1];
	m_updateObjRepo[m_validObjSizeinVector - 1] = nullptr;
	--m_validObjSizeinVector;

	//해쉬 속 기존 ItemA 요소 제거
	m_gameObjRepo.erase(inID);

	//해쉬 속 ItemB의 배열 인덱스 업데이트
	size_t oldID = m_updateObjRepo[arrayIdx]->GetObjectID();
	m_gameObjRepo[oldID].first = arrayIdx;
}

/* 가비지 컬렉터 */
inline void Scene::CleanGarbge()
{
	if (m_validObjSizeinVector < m_updateObjRepo.size())
	{
		m_updateObjRepo.erase(m_updateObjRepo.begin() + m_validObjSizeinVector, m_updateObjRepo.end());
	}
}


inline Scene::Scene()
{
	m_gameObjRepo.reserve(sReserveCapacity);
	m_updateObjRepo.reserve(sReserveCapacity);
}
