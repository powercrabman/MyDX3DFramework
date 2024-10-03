#pragma once

#include "GameObject.h"

class Scene
{
	class GameObjectStorage;
public:
	virtual ~Scene() = default;

	//�� ���� �Լ�
	void EnterSceneCore();
	void UpdateSceneCore();
	void ExitSceneCore();

	/* ������Ʈ ���� */
	template <typename ObjType, typename... Args>
	ObjType* CreateGameObject(Args&&...args);

	template <typename ObjType, typename... Args>
	ObjType* CreateNamedGameObject(std::string_view inObjectName, Args&&...args);

	/* Ž�� */
	GameObject* GetGameObjectOrNull(const CM::Name& inObjName);

	/* Ž��2 */
	GameObject* FindGameObjectOrNull(const std::function<bool(const GameObject*)>& inFindFunction);

	/* ���� */
	void RemoveGameObject(const CM::Name inObjName);
	void RemoveGameObject(GameObject* inObject);

	/* ������ �÷��� */
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

/* Ž�� */
inline GameObject* Scene::GetGameObjectOrNull(const CM::Name& inObjName)
{
	auto iter = m_gameObjRepo.find(inObjName);
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
inline void Scene::RemoveGameObject(const CM::Name inObjName)
{
	//������ ��� ã��
	auto iter = m_gameObjRepo.find(inObjName);
	assert(iter != m_gameObjRepo.end());

	//�迭�� �ε���
	size_t arrayIdx = (*iter).second.first;

	//���� ��Ұ� 1����� early return
	if (m_validObjSizeinVector == 1)
	{
		//�ؽ� �� ���� ItemA ��� ����
		m_updateObjRepo[0] = nullptr;
		m_gameObjRepo.erase(inObjName);
		m_validObjSizeinVector = 0;

		return;
	}

	//ItemA�� ItemB�� ã���� �ڸ� ����, ItemA�� nullptr�� ����
	m_updateObjRepo[arrayIdx] = m_updateObjRepo[m_validObjSizeinVector - 1];
	m_updateObjRepo[m_validObjSizeinVector - 1] = nullptr;
	--m_validObjSizeinVector;

	//�ؽ� �� ���� ItemA ��� ����
	m_gameObjRepo.erase(inObjName);

	//�ؽ� �� ItemB�� �迭 �ε��� ������Ʈ
	const CM::Name& oldName = m_updateObjRepo[arrayIdx]->GetName();
	m_gameObjRepo[oldName].first = arrayIdx;
}

inline void Scene::RemoveGameObject(GameObject* inObject)
{
	RemoveGameObject(inObject->GetName());
}

/* ������ �÷��� */
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
	//�迭�� ������Ʈ
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

	//�ؽ��� ������Ʈ
	const CM::Name& name = inObject->GetName();
	ASSERT(!m_gameObjRepo.contains(name), "������ ID�� ���� ������Ʈ�� �� ���� �̹� �����մϴ�.");
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
