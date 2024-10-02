#pragma once

namespace WinUtile
{
	inline void OutputLog(std::wstring_view msg)
	{
		::OutputDebugString(msg.data());
	}

	void Alert(std::wstring_view msg, std::wstring_view tag = L"Alert");
	void Alert(HWND hwnd, std::wstring_view msg, std::wstring_view tag = L"Alert");
}