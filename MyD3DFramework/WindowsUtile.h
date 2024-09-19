#pragma once

namespace WinUtile
{
	void Alert(const std::wstring& msg, const std::wstring& tag);
	void Alert(HWND hwnd, const std::wstring& msg, const std::wstring& tag);

	inline void OutputLog(const std::wstring& msg)
	{
		::OutputDebugString(msg.c_str());
	}
}