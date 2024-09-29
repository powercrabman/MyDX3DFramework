#pragma once

namespace CM
{
	struct TypeID
	{
		inline bool operator==(const TypeID& inOther) const
		{
			return Value == inOther.Value;
		}

		const void* Value;
	};

	class ITypeTrait
	{
	protected:
		ITypeTrait() = default;
		virtual ~ITypeTrait() = default;
	};

	template <
		typename Type,
		typename = std::is_class<Type>,
		typename = std::enable_if_t<!std::is_pointer_v<Type>>
	>
	class TypeTrait : public ITypeTrait
	{
	public:
		constexpr inline static TypeID ID()
		{
			return m_typeID;
		}

	private:
		constexpr static char m_value{};
		constexpr static TypeID m_typeID{ &m_value };
	};
}

//TypeID 해시 특수화
namespace std
{
	template<>
	struct hash<CM::TypeID>
	{
		size_t operator()(const CM::TypeID& typeId) const
		{
			return std::hash<const void*>{}(typeId.Value);
		}
	};
}

