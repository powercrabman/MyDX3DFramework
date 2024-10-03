#pragma once
#include "CBehavior.h"

class GameObject
{
	friend class Scene;
public:
	virtual ~GameObject() = default;

	virtual void InitalizeCore();
	virtual void UpdateCore();

	const CM::Name GetName() const { return m_name; }

	// 이름을 명시적으로 정하지 않을 경우
	template <typename CompType, typename ...Args>
	CompType* CreateComponent(Args&& ...args);

	// 이름을 명시적으로 정할 경우
	template<typename CompType, typename ...Args>
	CompType* CreateNamedComponent(std::string_view inCompName, Args && ...args);

	// 특정 컴포넌트가 존재하는가
	template<typename CompType>
	bool HasComponent();

	// 컴포넌트 클래스로 탐색
	template<typename CompType>
	CompType* GetComponentOrNull();

	// 컴포넌트 클래스 뭉치 탐색
	template<typename CompType>
	std::vector<std::unique_ptr<Component>>& GetComponents();

	// 컴포넌트 이름으로 탐색
	template<typename CompType>
	CompType* GetComponentByNameOrNull(const CM::Name& inCompName);

	//컴포넌트 삭제
	template<typename CompType>
	void RemoveComponentByName(const CM::Name& inCompName);

	//컴포넌트 클래스로 삭제
	template<typename CompType>
	void RemoveComponent();

	//updateComp 가비지 컬렉터
	void CleanGarbage();

protected:
	GameObject();

private:
	// 컴포넌트 생성 관련 함수
	void AddComponentToRepo(const CM::TypeID& inTypeID, std::unique_ptr<Component> inCmp);
	void AddCBehaviorHandler(CBehavior* inCmp);

private:
	void SetupObject(std::string_view inNameOrEmpty, bool isExplitName);

	virtual void Initialize() = 0;
	virtual void Update() = 0;

private:
	inline static uint64 sObjectIDCounter = 0;
	CM::Name m_name;

	/* 컴포넌트 관련 */
	constexpr static size_t sReserveCapacity = 16;
	std::unordered_map<CM::TypeID, std::vector<std::unique_ptr<Component>>> m_compRepo;
	std::vector<CBehavior*> m_updateCompRepo;
	size_t m_validCompSizeInVector = 0;
};

inline GameObject::GameObject()
{
	m_compRepo.reserve(sReserveCapacity);
	m_updateCompRepo.reserve(sReserveCapacity);
}


// 컴포넌트 생성 관련 함수
inline void GameObject::AddComponentToRepo(const CM::TypeID& inTypeID, std::unique_ptr<Component> inCmp)
{
	size_t idxInVector = m_validCompSizeInVector - 1;
	if (m_compRepo.contains(inTypeID))
	{
		m_compRepo[inTypeID].push_back(std::move(inCmp));
	}
	else
	{
		std::vector<std::unique_ptr<Component>> vec;
		vec.reserve(8);
		vec.push_back(std::move(inCmp));
		m_compRepo[inTypeID] = std::move(vec);
	}
}

inline void GameObject::AddCBehaviorHandler(CBehavior* inCmp)
{
	size_t endIdx = m_updateCompRepo.size();
	assert(endIdx >= m_validCompSizeInVector);

	if (endIdx == m_validCompSizeInVector)
	{
		m_updateCompRepo.push_back(inCmp);
	}
	else
	{
		m_updateCompRepo[m_validCompSizeInVector] = inCmp;
	}

	inCmp->SetIndex(m_validCompSizeInVector);
	++m_validCompSizeInVector;
}

inline void GameObject::SetupObject(std::string_view inNameOrEmpty, bool isExplitName)
{
	if (isExplitName)
	{
		m_name = CM::Name(inNameOrEmpty, CM::UNIQUE_KEY);
	}
	else
	{
		m_name = CM::Name(inNameOrEmpty, sObjectIDCounter++);
	}
}

inline void GameObject::CleanGarbage()
{
	if (m_validCompSizeInVector < m_updateCompRepo.size())
	{
		m_updateCompRepo.erase(m_updateCompRepo.begin() + m_validCompSizeInVector, m_updateCompRepo.end());
	}
}


// 이름을 명시적으로 정하지 않을 경우
template<typename CompType, typename ...Args>
inline CompType* GameObject::CreateComponent(Args && ...args)
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");

	std::unique_ptr<CompType> cmp = std::make_unique<CompType>(std::forward<Args>(args)...);
	CompType* ptr = cmp.get();

	//이름 설정
	cmp->SetupComponent<CompType>(this, CM::TypeTrait<CompType>::ID(), CM::GetTypeName<CompType>());
	cmp->Initialize();

	if constexpr (std::is_base_of<CBehavior, CompType>::value)
	{
		AddCBehaviorHandler(cmp.get());
	}

	AddComponentToRepo(CM::TypeTrait<CompType>::ID(), std::move(cmp));;
	return ptr;
}

// 이름을 명시적으로 정할 경우
template<typename CompType, typename ...Args>
inline CompType* GameObject::CreateNamedComponent(std::string_view inCompName, Args && ...args)
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");

	std::unique_ptr<CompType> cmp = std::make_unique<CompType>(std::forward<Args>(args)...);
	CompType* ptr = cmp.get();
	cmp->Initialize();

	//이름 설정
	cmp->SetupComponent<CompType>(this, CM::TypeTrait<CompType>::ID(), inCompName);

	if constexpr (std::is_base_of<CBehavior, CompType>::value)
	{
		AddCBehaviorHandler(cmp.get());
	}
	AddComponentToRepo(CM::TypeTrait<CompType>::ID(), std::move(cmp));

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

	if (iter == m_compRepo.end())
	{
		return nullptr;
	}

	return static_cast<CompType*>(iter->second[0].get());
}

template<typename CompType>
inline std::vector<std::unique_ptr<Component>>& GameObject::GetComponents()
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");
	auto iter = m_compRepo.find(CM::TypeTrait<CompType>::ID());
	ASSERT(iter != m_compRepo.end(), "GetComponents의 잘못된 사용");
	return iter->second;
}

template<typename CompType>
inline CompType* GameObject::GetComponentByNameOrNull(const CM::Name& inCompName)
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");
	auto& objVec = GetComponents<CompType>();

	for (auto& ptr : objVec)
	{
		if (ptr->GetName() == inCompName)
		{
			return ptr.get();
		}
	}

	return nullptr;
}

//컴포넌트 삭제
template<typename CompType>
inline void GameObject::RemoveComponentByName(const CM::Name& inCompName)
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

//컴포넌트 삭제
template<typename CompType>
inline void GameObject::RemoveComponent()
{
	static_assert(std::is_base_of<Component, CompType>::value, "CompType is not derived from Component.");

	std::vector<std::unique_ptr<Component>>& compTypeVec = GetComponents<CompType>();
	auto iterToDelete = compTypeVec.begin();

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

