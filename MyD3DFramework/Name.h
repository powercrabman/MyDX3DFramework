#pragma once

namespace CM
{
    constexpr size_t UNIQUE_KEY = UINT64_MAX;

    class Name
    {
        friend struct std::hash<Name>;
    public:

        Name() = default;
        Name(std::string_view inString, size_t inNumber);

        static Name GetUniqueName(std::string_view inString) { return Name{ inString, UNIQUE_KEY }; }

        bool operator==(const Name& inOther) const;

        std::string_view ToString() const;

    private:
        using Iterator = std::unordered_map<std::string, size_t>::iterator;
        inline static std::unordered_map<std::string, size_t> sNamePool;

        static Iterator FindOrAdd(std::string_view inName);

    private:
        std::string_view m_stringView;

        size_t m_nameID = NULL;
        size_t m_extraIndex = UNIQUE_KEY;
    };
}

inline CM::Name::Name(std::string_view inString, size_t inNumber)
{
    Iterator iter = FindOrAdd(inString);
    m_nameID = iter->second;
    m_stringView = iter->first;
    m_extraIndex = inNumber;
}

inline bool CM::Name::operator==(const Name& Other) const
{
    return m_nameID == Other.m_nameID && m_extraIndex == Other.m_extraIndex;
}

inline std::string_view CM::Name::ToString() const
{
    return m_stringView;
}

inline CM::Name::Iterator CM::Name::FindOrAdd(std::string_view inName)
{
    std::string str = inName.data();
    auto iter = sNamePool.find(str);
    if (iter != sNamePool.end())
    {
        return iter;
    }

    size_t newIndex = sNamePool.size();
    auto insertIter = sNamePool.emplace(str, newIndex);
    return insertIter.first;
}

namespace std
{
    template <>
    struct hash<CM::Name>
    {
        size_t operator()(const CM::Name& inItem) const
        {
            size_t hashValue = std::hash<size_t>()(inItem.m_nameID);
            hashValue ^= std::hash<size_t>()(inItem.m_extraIndex) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            return hashValue;
        }
    };
}