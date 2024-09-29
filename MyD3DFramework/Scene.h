#pragma once

#include "GameObject.h"

class Scene
{
	class GameObjectStorage;
public:
	virtual ~Scene() = default;

	//�� ���� �Լ�							
	void EnterSceneCore();
	void UpdateSceneCore(float inDeltaTime);
	void ExitSceneCore();

    template <typename ObjType, typename... Args>
	inline GameObject* CreateGameObject(Args&&...args);

	/* Ž�� */
	inline GameObject* GetGameObjectOrNull(uint64 inID);

	/* Ž��2 */
	inline GameObject* FindGameObjectOrNull(const std::function<bool(const GameObject*)>& inFindFunction);

	/* ���� */
	inline void RemoveGameObject(uint64 inID);

	/* ������ �÷��� */
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

	//�迭�� ������Ʈ
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

	//�ؽ��� ������Ʈ
	m_gameObjRepo[ptr->GetObjectID()] = std::make_pair(m_validObjSizeinVector - 1, std::move(obj));

	return ptr;
}

/* Ž�� */
inline GameObject* Scene::GetGameObjectOrNull(uint64 inID)
{
	auto iter = m_gameObjRepo.find(inID);
	if (iter != m_gameObjRepo.end())
	{
		return iter->second.second.get();
	}
	return nullptr;
}

/* Ž��2 */
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

/* ���� */
inline void Scene::RemoveGameObject(uint64 inID)
{
	//������ ��� ã��
	auto iter = m_gameObjRepo.find(inID);
	assert(iter != m_gameObjRepo.end());

	//�迭�� �ε���
	size_t arrayIdx = (*iter).second.first;

	//���� ��Ұ� 1����� early return
	if (m_validObjSizeinVector == 1)
	{
		//�ؽ� �� ���� ItemA ��� ����
		m_updateObjRepo[0] = nullptr;
		m_gameObjRepo.erase(inID);
		m_validObjSizeinVector = 0;

		return;
	}

	//ItemA�� ItemB�� ã���� �ڸ� ����, ItemA�� nullptr�� ����
	m_updateObjRepo[arrayIdx] = m_updateObjRepo[m_validObjSizeinVector - 1];
	m_updateObjRepo[m_validObjSizeinVector - 1] = nullptr;
	--m_validObjSizeinVector;

	//�ؽ� �� ���� ItemA ��� ����
	m_gameObjRepo.erase(inID);

	//�ؽ� �� ItemB�� �迭 �ε��� ������Ʈ
	size_t oldID = m_updateObjRepo[arrayIdx]->GetObjectID();
	m_gameObjRepo[oldID].first = arrayIdx;
}

/* ������ �÷��� */
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
