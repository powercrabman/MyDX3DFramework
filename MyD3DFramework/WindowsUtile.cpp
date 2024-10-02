#include "pch.h"
#include "Engine.h"
#include "WindowsApp.h"
#include "WindowsUtile.h"

void WinUtile::Alert(std::wstring_view msg, std::wstring_view tag)
{
	HWND hwnd = WindowsApp::GetInst().GetHwnd();
	::MessageBox(hwnd, msg.data(), tag.data(), MB_OK);
}

void WinUtile::Alert(HWND hwnd, std::wstring_view msg, std::wstring_view tag)
{
	::MessageBox(hwnd, msg.data(), tag.data(), MB_OK);
}
