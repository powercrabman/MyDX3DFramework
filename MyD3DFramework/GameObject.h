#pragma once
#include "CBehavior.h"

class GameObject
{
	friend class Renderer; /* ���߿� ObjectManager�� ���� */

public:
	GameObject();
	virtual ~GameObject() = default;

	virtual void InitalizeCore();
	virtual void UpdateCore(float inDeltaTime);

	inline uint64 GetObjectID() const { return m_objectID; }
	inline CM::TypeInfo GetTypeInfo() const { return m_typeInfo; }

	template <typename CompTy, typename ...Args>
	inline CompTy* RegisterComponent(const std::wstring& inStringKey, Args ...args);

	template <typename CompTy>
	inline CompTy* GetComponent(const std::wstring& inStringKey);

	template <typename CompTy>
	inline bool HasComponent();

private:
	virtual void Initialize() = 0;
	virtual void Update(float inDeltaTime) = 0;

	inline void SetTypeInfo(const CM::TypeInfo& inTypeInfo);

private:
	static uint64 ObjectIDCounter;
	uint64 m_objectID = 0;

	CM::TypeInfo m_typeInfo = {};

	//������Ʈ�� �����ϴ� �����̳�
	using StrCompPair = std::pair<std::wstring, std::unique_ptr<Component>>;
	std::unordered_map<uint64, std::vector<StrCompPair>> m_compRepo{ 16 };
	std::vector<CBehavior*> m_updateCompRepo{ 16 };
};

template<typename CompTy, typename ...Args>
inline CompTy* GameObject::RegisterComponent(const std::wstring& inStringKey, Args ...args)
{
	static_assert(std::is_base_of<Component, CompTy>::value, "������Ʈ ������ �ƴմϴ�.");
	static_assert(!std::is_same<Component, CompTy>::value, "Component�� ���� ������ �� �����ϴ�.");

	TypeInfo tInfo = CM::TypeTrait<CompTy>::GetInfo();
	assert(!m_compRepo.contains(tInfo.ID) && "�̹� �ش� Ÿ���� ������Ʈ�� �����մϴ�.");

	// ������Ʈ ����
	std::unique_ptr<CompTy> cmp = std::make_unique<CompTy>(std::forward<Args>(args)...);
	CompTy* cPtr = cmp.get();

	// ������ �� Ÿ�� ���� ����
	cmp->SetOwner(this);
	cmp->SetTypeInfo(tInfo);

	// �ش� Ÿ���� ������Ʈ�� ���ٸ� ���� �߰�
	auto repo = m_compRepo.find(tInfo.ID);
	if (repo == m_compRepo.end())
	{
		std::vector<StrCompPair> vec{ 8 };
		vec.emplace_back(std::make_pair(inStringKey, std::move(cmp)));
		m_compRepo[tInfo.ID] = std::move(vec);
	}
	else
	{
		repo->second.emplace_back(std::make_pair(inStringKey, std::move(cmp)));
	}

	// ���� CBehavior���� ��ӹ��� Ÿ���̸� ������Ʈ ��Ͽ� �߰�
	if constexpr (std::is_base_of<CBehavior, CompTy>::value)
	{
		m_updateCompRepo.push_back(static_cast<CBehavior*>(cPtr));
	}

	return cPtr;
}

template<typename CompTy>
inline CompTy* GameObject::GetComponent(const std::wstring& inStringKey)
{
	static_assert(std::is_base_of<Component, CompTy>::value, "������Ʈ ������ �ƴմϴ�.");
	ASSERT(HasComponent<CompTy>(), "������Ʈ�� �������� �ʽ��ϴ�.");

	const auto& repo = m_compRepo[TypeTrait<CompTy>::GetInfo().ID];

	for (const auto& compPair : repo)
	{
		if (compPair.first == inStringKey)
		{
			Component* cmp = compPair.second.get();
			assert(CM::TypeTrait<CompTy>::GetInfo() == cmp->GetTypeInfo() && "������Ʈ�� Ÿ���� ��ġ���� �ʽ��ϴ�.");
			return static_cast<CompTy*>(cmp);
		}
	}

	assert(false, "������Ʈ�� �������� �ʽ��ϴ�.");
	return nullptr;
}

template<typename CompTy>
inline bool GameObject::HasComponent()
{
	return m_compRepo.contains(TypeTrait<CompTy>::GetInfo().ID);
}

inline GameObject::GameObject()
{
	m_objectID = ++ObjectIDCounter;
}

inline void GameObject::SetTypeInfo(const CM::TypeInfo& inTypeInfo)
{
	m_typeInfo = inTypeInfo;
}
