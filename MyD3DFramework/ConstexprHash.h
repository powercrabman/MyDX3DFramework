#pragma once

namespace CM
{
	template<std::size_t N>
	struct StringHashHelper
	{
		char String[N];
		size_t Hash;
		size_t Size = N;

		constexpr StringHashHelper(const char(&str)[N])
		{
			for (size_t i = 0; i < N; ++i)
			{
				String[i] = str[i];
			}
			Hash = FNV1a(String, N - 1);
		}

		constexpr size_t FNV1a(const char* str, size_t length, size_t hash = 0x811C9DC5) const
		{
			return length == 0 ? hash : FNV1a(&str[1], length - 1, (hash ^ str[0]) * 0x01000193);
		}
	};

	template<StringHashHelper Helper>
	struct StringHash
	{
		char String[Helper.Size];
		size_t Hash = Helper.Hash;

		constexpr StringHash()
		{
			for (size_t i = 0; i < Helper.Size; ++i)
			{
				String[i] = Helper.String[i];
			}
		}
	};
}