#pragma once
#include "CBehavior.h"

class GameObject
{
	friend class Scene;
public:
	inline GameObject();
	virtual ~GameObject() = default;

	virtual void InitalizeCore();
	virtual void UpdateCore(float inDeltaTime);

	inline uint64 GetObjectID() const { return m_objectID; }

	//컴포넌트 생성
	template <typename CompType, typename ...Args>
	inline CompType* CreateComponent(Args&& ...args, const std::wstring& inCompName);

	template<typename CompType>
	inline bool HasComponent();
	
	template<typename CompType>
	inline CompType* GetComponentOrNull();

	template<typename CompType>
	inline std::vector<std::unique_ptr<Component>>& GetComponents();

	template<typename CompType>
	inline CompType* GetComponentByNameOrNull(const std::wstring& inName);

	//컴포넌트 삭제
	template<typename CompType>
	inline void RemoveComponent(const std::wstring& inCompName);

	//updateComp 가비지 컬렉터
	inline void CleanGarbage();

private:
	virtual void Initialize() = 0;
	virtual void Update(float inDeltaTime) = 0;

private:
	inline static uint64 ObjectIDCounter = 0;
	uint64 m_objectID = 0;

	//컴포넌트를 저정하는 컨테이너
	std::unordered_map<CM::TypeID, std::vector<std::unique_ptr<Component>>> m_compRepo;
	std::vector<CBehavior*> m_updateCompRepo;
	size_t m_validCompSizeInVector = 0;
};


inline GameObject::GameObject()
{
	m_objectID = ObjectIDCounter++;
}


inline void GameObject::CleanGarbage()
{
	if (m_validCompSizeInVector < m_updateCompRepo.size())
	{
		m_updateCompRepo.erase(m_updateCompRepo.begin() + m_validCompSizeInVector, m_updateCompRepo.end());
	}
}

//컴포넌트 생성
template<typename CompType, typename ...Args>
inline CompType* GameObject::CreateComponent(Args && ...args, const std::wstring& inCompName)
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");

	std::unique_ptr<CompType> cmp = std::make_unique<CompType>(std::forward<Args>(args)...);
	cmp->InitComponent<CompType>(this, inCompName);
	CompType* ptr = cmp.get();

	// CBehavior 인 경우 처리
	if constexpr (std::is_base_of<CBehavior, CompType>::value)
	{
		size_t endIdx = m_updateCompRepo.size();
		assert(endIdx >= m_validCompSizeInVector);

		if (endIdx == m_validCompSizeInVector)
		{
			m_updateCompRepo.push_back(ptr);
		}
		else
		{
			m_updateCompRepo[m_validCompSizeInVector] = ptr;
		}

		ptr->SetIndex(m_validCompSizeInVector);
		++m_validCompSizeInVector;
	}

	// 해시맵에 등록
	size_t idxInVector = m_validCompSizeInVector - 1;
	if (HasComponent<CompType>())
	{
		m_compRepo[CM::TypeTrait<CompType>::ID()].push_back(std::move(cmp));
	}
	else
	{
		std::vector<std::unique_ptr<Component>> vec;
		vec.reserve(8);
		vec.push_back(std::move(cmp));
		m_compRepo[CM::TypeTrait<CompType>::ID()] = std::move(vec);
	}

	return ptr;
}

//특정 타입의 컴포넌트가 존재하는가
template<typename CompType>
inline bool GameObject::HasComponent()
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");
	return m_compRepo.contains(CM::TypeTrait<CompType>::ID());
}

template<typename CompType>
inline CompType* GameObject::GetComponentOrNull()
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");
	auto iter = m_compRepo.find(CM::TypeTrait<CompType>::ID());

	if (iter == m_compRepo.end() || iter->second.empty())
	{
		return nullptr;
	}

	return iter->second[0].get();
}

template<typename CompType>
inline std::vector<std::unique_ptr<Component>>& GameObject::GetComponents()
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");
	auto iter = m_compRepo.find(CM::TypeTrait<CompType>::ID());
	VERTIFY(iter != m_compRepo.end(), "GetComponents의 잘못된 사용");
	return iter->second;
}

template<typename CompType>
inline CompType* GameObject::GetComponentByNameOrNull(const std::wstring& inName)
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");
	auto& objVec = GetComponents<CompType>();

	for (auto& ptr : objVec)
	{
		if (ptr->GetName() == inName)
		{
			return ptr.get();
		}
	}

	return nullptr;
}

template<typename CompType>
inline void GameObject::RemoveComponent(const std::wstring& inCompName)
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");

	std::vector<std::unique_ptr<Component>>& compTypeVec = GetComponents<CompType>();
	auto iterToDelete = std::find_if(compTypeVec.begin(), compTypeVec.end(),
		[&inCompName](const std::unique_ptr<Component>& inOther)
		{
			return inOther->GetName() == inCompName;
		}
	);

	if (iterToDelete == compTypeVec.end())
	{
		return;
	}

	Component* cmpToDelete = iterToDelete->get();
	Component* successor = nullptr;

	if constexpr (std::is_base_of<CBehavior, CompType>::value)
	{
		size_t updateCompRepoSize = m_updateCompRepo.size();

		assert(updateCompRepoSize > 0);

		if (updateCompRepoSize == 1)
		{
			m_updateCompRepo[0] = nullptr;
			m_validCompSizeInVector = 0;
		}
		else
		{
			successor = m_updateCompRepo[m_validCompSizeInVector - 1];
			m_updateCompRepo[cmpToDelete->GetIndex()] = successor;
			m_updateCompRepo[m_validCompSizeInVector - 1] = nullptr;
			--m_validCompSizeInVector;
		}
	}

	if (successor)
	{
		successor->SetIndex(cmpToDelete->GetIndex());
	}

	compTypeVec.erase(iterToDelete);
}
