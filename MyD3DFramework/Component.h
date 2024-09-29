#pragma once

class GameObject;

class Component
{
    friend class GameObject;

public:
    virtual ~Component() = default;
    inline GameObject* GetOwner() const
    {
        assert(m_owner);
        return m_owner;
    }
    inline CM::TypeID GetTypeInfo() const { return m_typeID; }

    inline void Enable() { m_enable = true; }
    inline void Disable() { m_enable = false; }
    inline bool IsEnable() const { return m_enable; }

    inline const std::wstring& GetName() const;

protected:
    Component();

private:
    template<typename CompType>
    inline void InitComponent(
        GameObject* inOwner,
        const std::wstring& inName
    )
    {
        //생성자 간략화를 위해서 사용
        assert(inOwner);
        m_name = inName;
        m_owner = inOwner;
        m_typeID = CM::TypeTrait<CompType>::ID();
    }

    inline void SetIndex(size_t inIdx);
    inline size_t GetIndex();

private:
    std::wstring m_name = {};
    size_t m_index = UINT64_MAX;

    CM::TypeID m_typeID = {};
    GameObject* m_owner = nullptr;

    bool m_enable = true;
};

inline const std::wstring& Component::GetName() const
{
    return m_name;
}

inline Component::Component()
{
}

inline void Component::SetIndex(size_t inIdx)
{
    m_index = inIdx;
}

inline size_t Component::GetIndex()
{
    return m_index;
}

