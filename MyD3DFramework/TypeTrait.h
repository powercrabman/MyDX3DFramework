#pragma once

namespace CM
{
    struct TypeInfo
    {
        inline bool operator==(const TypeInfo& inOther) const
        {
            return ID == inOther.ID
                && TypeName == inOther.TypeName;
        }

        uint64 ID = {};
        const char* TypeName = {};
    };

    class TypeTraitBase
    {
    protected:
        static uint64 InstTypeID()
        {
            uint64 result = m_currentTypeID;
            m_currentTypeID += ID_COUNT_STEP;

            return result;
        }

    private:
        enum { ID_COUNT_STEP = 1 };
        inline static size_t m_currentTypeID = 0;
    };

    template <typename Type>
    class TypeTrait : TypeTraitBase
    {
    public:
        static TypeInfo GetInfo()
        {
            static TypeInfo trait = []() {
                TypeInfo result = {};
                result.ID = InstTypeID();
                result.TypeName = typeid(Type);

                return result;
                };

            return trait;
        }
    };
}