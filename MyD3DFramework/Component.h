#pragma once

class GameObject;
class CBehavior;

class Component
{
	friend class GameObject;
public:
	virtual ~Component() = default;
	virtual void Initialize() {}

	GameObject* GetOwner() const;
	CM::TypeID GetTypeInfo() const { return m_typeID; }

	void Enable() { m_enable = true; }
	void Disable() { m_enable = false; }
	bool IsEnable() const { return m_enable; }
	bool IsUpdateable() const { return m_isUpdateable; }

	const CM::Name GetName() const { return m_name; }

	template<typename CompType>
	CompType* CastOrNull();

protected:
	Component() = default;

private:
	template<typename CompType>
	void SetupComponent(
		GameObject* inOwner,
		const CM::TypeID& inTypeID,
		std::string_view inName
	);

	void SetIndex(size_t inIdx);
	size_t GetIndex() const;

private:
	inline static uint32 sComponentIDCounter = 0;
	CM::Name m_name;

	size_t m_index = UINT64_MAX; //����� Component���� ���Խ�Ű�� �ʴ°� ����, �̴� ���� ����� ����� �и��� �� ����

	CM::TypeID m_typeID = {}; /* �̰� ����? */
	GameObject* m_owner = nullptr;

	bool m_enable = true;
	bool m_isUpdateable = false;

};

inline GameObject* Component::GetOwner() const
{
	assert(m_owner);
	return m_owner;
}

inline void Component::SetIndex(size_t inIdx)
{
	m_index = inIdx;
}

inline size_t Component::GetIndex() const
{
	return m_index;
}

template<typename CompType>
inline CompType* Component::CastOrNull()
{
	static_assert(std::is_base_of<Component, CompType>::value);

	if (m_typeID == CM::TypeTrait<CompType>::ID())
	{
		return static_cast<CompType*>(this);
	}

	return nullptr;
}

template<typename CompType>
inline void Component::SetupComponent(GameObject* inOwner, const CM::TypeID& inTypeID, std::string_view inName)
{
	assert(inOwner);

	m_name = CM::Name{ inName, sComponentIDCounter++ };
	m_owner = inOwner;
	m_typeID = inTypeID;
	m_isUpdateable = std::is_base_of<CBehavior, CompType>::value;
}
