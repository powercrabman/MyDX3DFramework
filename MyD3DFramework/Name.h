#pragma once

namespace CM
{
    class Name
    {
        friend struct std::hash<Name>;
    public:
        inline static std::unordered_map<std::string, size_t> sNamePool;

        inline Name(const std::string& InString, size_t InNumber = 0);
        inline bool operator==(const Name& Other) const;

        inline std::string_view ToString() const;

    private:
        inline static size_t FindOrAddName(const std::string& inName);

    private:
        std::string_view m_string;

        size_t m_nameID;
        size_t m_additionalNumber;
    };
}

namespace std
{
    template <>
    struct hash<CM::Name>
    {
        size_t operator()(const CM::Name& inItem) const
        {
            size_t hashValue = std::hash<size_t>()(inItem.m_nameID);
            hashValue ^= std::hash<size_t>()(inItem.m_additionalNumber) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            return hashValue;
        }
    };
}

inline CM::Name::Name(const std::string& inString, size_t inAdditionalNumber)
{
    m_nameID = FindOrAddName(inString);
    m_additionalNumber = inAdditionalNumber;
    m_string = inString;
}

inline bool CM::Name::operator==(const Name& Other) const
{
    return m_nameID == Other.m_nameID && m_additionalNumber == Other.m_additionalNumber;
}

inline std::string_view CM::Name::ToString() const
{
    return m_string;
}

inline size_t CM::Name::FindOrAddName(const std::string& Name)
{
    for (const auto& pair : sNamePool)
    {
        if (pair.first == Name)
        {
            return pair.second;
        }
    }
    std::string nameCopy(Name);
    size_t newIndex = sNamePool.size();
    sNamePool[nameCopy] = newIndex;
    return newIndex;
}
