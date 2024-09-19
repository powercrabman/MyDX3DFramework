#include "pch.h"
#include "Engine.h"
#include "WindowsApp.h"
#include "WindowsUtile.h"

void WinUtile::Alert(const std::wstring& msg, const std::wstring& tag = L"Alert")
{
	HWND hwnd = WindowsApp::GetInst().GetHwnd();
	::MessageBox(hwnd, msg.c_str(), tag.c_str(), MB_OK);
}

void WinUtile::Alert(HWND hwnd, const std::wstring& msg, const std::wstring& tag = L"Alert")
{
	::MessageBox(hwnd, msg.c_str(), tag.c_str(), MB_OK);
}
