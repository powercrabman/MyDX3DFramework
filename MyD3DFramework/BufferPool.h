#pragma once

template <size_t SIZE>
class BufferPoolW final
{
	DEFINE_SINGLE(BufferPoolW<SIZE>);
private:
	BufferPoolW() = default;
	~BufferPoolW() = default;

public:
	constexpr size_t Size() const { return SIZE; }

	inline void Input(const std::wstring& inStr)
	{
		size_t length = min(inStr.size(), SIZE - 1); 
		::memcpy(m_buffer, inStr.c_str(), length * sizeof(wchar_t));
		m_buffer[length] = L'\0';
	}

	inline void Input(const wchar_t* format, ...)
	{
		va_list args = {};
		va_start(args, format);
		vswprintf_s(m_buffer, SIZE, format, args);
		va_end(args);
	}

	inline void Clear()
	{
		::memset(m_buffer, L'\0', size_of(m_buffer));
	}

	inline void Append(const wchar_t* format, ...)
	{
		size_t currentLength = wcslen(m_buffer);

		if (currentLength >= SIZE)
		{
			return;
		};

		va_list args;
		va_start(args, format);
		vswprintf_s(m_buffer + currentLength, SIZE - currentLength, format, args);
		va_end(args);
	}

	inline void Append(const std::wstring& inStr)
	{
		size_t currentLength = wcslen(m_buffer);
		size_t remainSpace = SIZE - currentLength - 1;
		size_t lengthToCopy = min(inStr.size(), remainSpace);

		::memcpy(m_buffer + currentLength, inStr.c_str(), lengthToCopy * sizeof(wchar_t));
		m_buffer[currentLength + lengthToCopy] = L'\0';
	}

	constexpr inline wchar_t& operator[](size_t idx)
	{
		VERTIFY(idx < SIZE, L"¹öÆÛ ÀÎµ¦½Ì ¿À·ù.");
		return m_buffer[idx];
	}

	constexpr inline const wchar_t* c_str() const
	{
		return m_buffer;
	}

private:
	wchar_t m_buffer[SIZE] = {};
};

template <size_t SIZE>
class BufferPool final
{
	DEFINE_SINGLE(BufferPool<SIZE>);
private:
	BufferPool() = default;
	~BufferPool() = default;

public:
	constexpr size_t Size() const { return SIZE; }

	inline void Input(const std::string& inStr)
	{
		size_t length = min(inStr.size(), SIZE - 1);
		::memcpy(m_buffer, inStr.c_str(), length * sizeof(char));
		m_buffer[length] = L'\0';
	}

	inline void Input(const char* format, ...)
	{
		va_list args = {};
		va_start(args, format);
		vswprintf_s(m_buffer, SIZE, format, args);
		va_end(args);
	}

	inline void Clear()
	{
		::memset(m_buffer, L'\0', size_of(m_buffer));
	}

	inline void Append(const char* format, ...)
	{
		size_t currentLength = wcslen(m_buffer);

		if (currentLength >= SIZE)
		{
			return;
		};

		va_list args;
		va_start(args, format);
		vswprintf_s(m_buffer + currentLength, SIZE - currentLength, format, args);
		va_end(args);
	}

	inline void Append(const std::string& inStr)
	{
		size_t currentLength = wcslen(m_buffer);
		size_t remainSpace = SIZE - currentLength - 1;
		size_t lengthToCopy = min(inStr.size(), remainSpace);

		::memcpy(m_buffer + currentLength, inStr.c_str(), lengthToCopy * sizeof(char));
		m_buffer[currentLength + lengthToCopy] = L'\0';
	}

	constexpr inline char& operator[](size_t idx)
	{
		VERTIFY(idx < SIZE, L"¹öÆÛ ÀÎµ¦½Ì ¿À·ù.");
		return m_buffer[idx];
	}

	constexpr inline const char* c_str() const
	{
		return m_buffer;
	}

private:
	char m_buffer[SIZE] = {};
};

using Buffer512W = BufferPoolW<512>;
using Buffer512 = BufferPool<512>;
