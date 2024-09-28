#pragma once

class CBehavior;

class GameObject
{
	friend class Scene;
public:
	GameObject();
	virtual ~GameObject() = default;

	virtual void InitalizeCore();
	virtual void UpdateCore(float inDeltaTime);

	inline uint64 GetObjectID() const { return m_objectID; }
	//inline CM::TypeInfo GetTypeInfo() const { return m_typeInfo; }

	//template <typename CompTy, typename ...Args>
	//inline CompTy* RegisterComponent(const std::wstring& inStringKey, Args ...args)
	//{
	//	static_assert(std::is_base_of<Component, CompTy>::value, "컴포넌트 형식이 아닙니다.");
	//	static_assert(!std::is_same<Component, CompTy>::value, "Component는 직접 생성할 수 없습니다.");
	//
	//	TypeInfo tInfo = CM::TypeTrait<CompTy>::GetInfo();
	//	assert(!m_compRepo.contains(tInfo.ID) && "이미 해당 타입의 컴포넌트가 존재합니다.");
	//
	//	// 컴포넌트 생성
	//	std::unique_ptr<CompTy> cmp = std::make_unique<CompTy>(std::forward<Args>(args)...);
	//	CompTy* cPtr = cmp.get();
	//
	//	// 소유자 및 타입 정보 설정
	//	cmp->SetOwner(this);
	//	cmp->SetTypeInfo(tInfo);
	//
	//	// 해당 타입의 컴포넌트가 없다면 새로 추가
	//	auto repo = m_compRepo.find(tInfo.ID);
	//	if (repo == m_compRepo.end())
	//	{
	//		std::vector<StrCompPair> vec{ 8 };
	//		vec.emplace_back(std::make_pair(inStringKey, std::move(cmp)));
	//		m_compRepo[tInfo.ID] = std::move(vec);
	//	}
	//	else
	//	{
	//		repo->second.emplace_back(std::make_pair(inStringKey, std::move(cmp)));
	//	}
	//
	//	// 만약 CBehavior에서 상속받은 타입이면 업데이트 목록에 추가
	//	if constexpr (std::is_base_of<CBehavior, CompTy>::value)
	//	{
	//		m_updateCompRepo.push_back(static_cast<CBehavior*>(cPtr));
	//	}
	//
	//	return cPtr;
	//}
	//
	//template <typename CompTy>
	//inline CompTy* GetComponent(const std::wstring& inStringKey)
	//{
	//	static_assert(std::is_base_of<Component, CompTy>::value, "컴포넌트 형식이 아닙니다.");
	//	ASSERT(HasComponent<CompTy>(), "컴포넌트가 존재하지 않습니다.");
	//
	//	const auto& repo = m_compRepo[TypeTrait<CompTy>::GetInfo().ID];
	//
	//	for (const auto& compPair : repo)
	//	{
	//		if (compPair.first == inStringKey)
	//		{
	//			Component* cmp = compPair.second.get();
	//			assert(CM::TypeTrait<CompTy>::GetInfo() == cmp->GetTypeInfo() && "컴포넌트의 타입이 일치하지 않습니다.");
	//			return static_cast<CompTy*>(cmp);
	//		}
	//	}
	//
	//	assert(false, "컴포넌트가 존재하지 않습니다.");
	//	return nullptr;
	//}
	//
	//template <typename CompTy>
	//inline bool HasComponent()
	//{
	//	return m_compRepo.contains(TypeTrait<CompTy>::GetInfo().ID);
	//}

private:
	virtual void Initialize() = 0;
	virtual void Update(float inDeltaTime) = 0;

	//inline void SetTypeInfo(const CM::TypeInfo& inTypeInfo);

private:
	inline static uint64 ObjectIDCounter = 0;
	uint64 m_objectID = 0;

	//컴포넌트를 저정하는 컨테이너
	std::unordered_map<CM::TypeID, std::vector<std::unique_ptr<Component>>> m_compRepo;
	std::vector<CBehavior*> m_updateCompRepo;
};

//inline GameObject::GameObject()
//{
//	m_objectID = ++ObjectIDCounter;
//
//	m_compRepo.reserve(16);
//	m_updateCompRepo.reserve(16);
//}

//inline void GameObject::SetTypeInfo(const CM::TypeInfo& inTypeInfo)
//{
//	m_typeInfo = inTypeInfo;
//}
