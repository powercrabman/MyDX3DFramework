#pragma once

class GameObject;
class CBehavior;

class Component
{
	friend class GameObject;
public:
	virtual ~Component() = default;

	GameObject* GetOwner() const;
	CM::TypeID GetTypeInfo() const { return m_typeID; }

	void Enable() { m_enable = true; }
	void Disable() { m_enable = false; }
	bool IsEnable() const { return m_enable; }
	bool IsUpdateable() const { return m_isUpdateable; }

	const CM::Name GetName() const { return m_name; }

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

	size_t m_index = UINT64_MAX; //설계상 Component에는 포함시키지 않는게 맞음, 이는 현재 설계상 충분히 분리할 수 있음

	CM::TypeID m_typeID = {}; /* 이거 쓰나? */
	GameObject* m_owner = nullptr;

	bool m_enable = true;
	bool m_isUpdateable = false;

};

GameObject* Component::GetOwner() const
{
	assert(m_owner);
	return m_owner;
}

void Component::SetIndex(size_t inIdx)
{
	m_index = inIdx;
}

size_t Component::GetIndex() const
{
	return m_index;
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
