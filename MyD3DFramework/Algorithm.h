#pragma once

namespace CM
{
	inline std::string WstringToString(const std::wstring& inStr)
    {
        if (inStr.empty()) return std::string();

        int size = WideCharToMultiByte(CP_UTF8, 0, &inStr[0], (int)inStr.size(), NULL, 0, NULL, NULL);

        std::string result(size, 0);
        WideCharToMultiByte(CP_UTF8, 0, &inStr[0], (int)inStr.size(), &result[0], size, NULL, NULL);

        return result;
    }

	inline std::wstring StringToWstring(const std::string& inStr)
    {
        if (inStr.empty()) return std::wstring();

        int size = MultiByteToWideChar(CP_UTF8, 0, &inStr[0], (int)inStr.size(), NULL, 0);

        std::wstring result(size, 0);
        MultiByteToWideChar(CP_UTF8, 0, &inStr[0], (int)inStr.size(), &result[0], size);

        return result;
    }
}