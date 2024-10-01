#pragma once

class GameObject;
class CBehavior;

class Component
{
    friend class GameObject;
public:
    virtual ~Component() = default;

    inline GameObject* GetOwner() const;
    inline CM::TypeID GetTypeInfo() const { return m_typeID; }

    inline void Enable() { m_enable = true; }
    inline void Disable() { m_enable = false; }
    inline bool IsEnable() const { return m_enable; }
    inline bool IsUpdateable() const { return m_isUpdateable; }

    inline const std::wstring& GetName() const;
    inline const uint64 GetComponentID() const { return m_cmpID; }

protected:
    Component();

private:
    template<typename CompType>
    inline void InitComponent(
        GameObject* inOwner,
        const std::wstring& inName
    );

    inline void SetIndex(size_t inIdx);
    inline size_t GetIndex() const;

private:
    inline static uint32 sComponentIDCounter = 0;
    uint32 m_cmpID = 0;

    size_t m_index = UINT64_MAX; //설계상 Component에는 포함시키지 않는게 맞음, 이는 현재 설계상 충분히 분리할 수 있음

    CM::TypeID m_typeID = {};
    GameObject* m_owner = nullptr;

    bool m_enable = true;
    bool m_isUpdateable = false;
};

inline GameObject* Component::GetOwner() const
{
    assert(m_owner);
    return m_owner;
}

inline const std::wstring& Component::GetName() const
{
    return m_name;
}

inline Component::Component()
{
    m_cmpID = sComponentIDCounter++;
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
inline void Component::InitComponent(GameObject* inOwner, const std::wstring& inName)
{
    //생성자 간략화를 위해서 사용
    assert(inOwner);
    m_name = inName;
    m_owner = inOwner;
    m_typeID = CM::TypeTrait<CompType>::ID();
    m_isUpdateable = std::is_base_of<CBehavior, CompType>::value;
}
