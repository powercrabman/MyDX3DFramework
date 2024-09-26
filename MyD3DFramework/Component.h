#pragma once

class GameObject;

class Component
{
   //template <typename CompTy, typename ...Args>
   //friend inline CompTy* GameObject::RegisterComponent(const std::wstring& inHashKey, Args ...args);
    friend class GameObject;

public:
    virtual ~Component() = default;
    inline GameObject* GetOwner() const
    {
        assert(m_owner);
        return m_owner;
    }
    inline CM::TypeInfo GetTypeInfo() const { return m_typeInfo; }
    inline uint64 GetComponentID() const { return m_componentID; }

    inline void Enable() { m_enable = true; }
    inline void Disable() { m_enable = false; }
    inline bool IsEnable() const { return m_enable; }

protected:
    Component();

private:
    inline void SetTypeInfo(const CM::TypeInfo& inTypeInfo);
    inline void SetOwner(GameObject* inOwner);

private:
    static uint64 ComponentIDCounter;
    uint64 m_componentID = 0;

    CM::TypeInfo m_typeInfo = {};
    GameObject* m_owner = nullptr;

    bool m_enable = true;
};

inline Component::Component()
{
    m_componentID = ++ComponentIDCounter;
}

inline void Component::SetTypeInfo(const CM::TypeInfo& inTypeInfo)
{
    m_typeInfo = inTypeInfo;
}

inline void Component::SetOwner(GameObject* inOwner)
{
    assert(inOwner);
    m_owner = inOwner;
}
