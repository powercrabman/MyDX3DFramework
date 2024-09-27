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

    inline void Enable() { m_enable = true; }
    inline void Disable() { m_enable = false; }
    inline bool IsEnable() const { return m_enable; }

protected:
    Component();

private:
    virtual inline void InitComponent(
        const CM::TypeInfo& inTypeInfo,
        GameObject* inOwner,
        const std::wstring& inName
    ) final;

private:
    std::wstring m_name = {};

    CM::TypeInfo m_typeInfo = {};
    GameObject* m_owner = nullptr;

    bool m_enable = true;
};

inline Component::Component()
{
}

inline void Component::InitComponent(const CM::TypeInfo& inTypeInfo, GameObject* inOwner, const std::wstring& inName)
{
    //생성자 간략화를 위해서 사용

    assert(inOwner);

    m_name = inName;
    m_owner = inOwner;
    m_typeInfo = inTypeInfo;
}

