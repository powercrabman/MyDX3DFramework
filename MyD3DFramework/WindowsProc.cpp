#include "pch.h"
#include "resource.h"
#include "WindowsProc.h"
#include "WindowsApp.h"
#include "Engine.h"

std::wstring WinProc::ReadDlg(HWND parentHwnd, int id)
{
	HWND hwnd = GetDlgItem(parentHwnd, id);

	wchar_t buffer[256] = {};
	GetWindowText(hwnd, buffer, 256);

	return buffer;
}

std::wstring WinProc::ReadDlg(HWND dlgHwnd)
{
	wchar_t buffer[256] = {};
	GetWindowText(dlgHwnd, buffer, 256);

	return buffer;
}

LRESULT WinProc::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInst = WindowsApp::GetInst().GetHInstance();

	switch (message)
	{
	case WM_RBUTTONDOWN:
	{
		HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_MENU_TO_SETTING));

		if (hMenu)
		{
			HMENU hPopupMenu = GetSubMenu(hMenu, 0);
			POINT cursorPos;

			// 현재 커서 위치 가져오기
			GetCursorPos(&cursorPos);

			// 메뉴를 마우스 커서 위치에 표시
			TrackPopupMenu(hPopupMenu, TPM_RIGHTBUTTON, cursorPos.x, cursorPos.y, 0, hwnd, NULL);

			DestroyMenu(hMenu);
		}
		return TRUE;
	}

	case WM_COMMAND:
	{
		int cmd = LOWORD(wParam);

		switch (cmd)
		{
		case ID_SETTING_RESIZE:
			DialogBox(hInst, MAKEINTRESOURCE(ID_RESIZE_DLG), hwnd, ResizeProc::ResizeProc);
			return TRUE;
		}

		break;
	}

	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			//나의 윈도우를 사용하지 않을 경우, sleep을 걸어서 OS에게 CPU 사이클을 돌려준다.
			Engine::GetInst().SetAppPauseState(true);
		}
		else
		{
			Engine::GetInst().SetAppPauseState(false);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
	{
		Engine::GetInst().SetAppPauseState(false);
		RECT winRect = {};
		GetClientRect(WindowsApp::GetInst().GetHwnd(), &winRect);

		WindowsApp::GetInst().ResizeWindow(winRect);
	}
	return 0;

	case WM_ENTERSIZEMOVE:
		Engine::GetInst().SetAppPauseState(true);
		return 0;

	case WM_EXITSIZEMOVE:
	{
		Engine::GetInst().SetAppPauseState(false);
		RECT winRect = {};
		GetClientRect(WindowsApp::GetInst().GetHwnd(), &winRect);

		WindowsApp::GetInst().ResizeWindow(winRect);
	}
	return 0;

	case WM_GETMINMAXINFO:
		/* 사이즈가 너무 작아지는 것을 방지 */
		reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = 200;
		reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = 200;
		return 0;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT WinProc::ResizeProc::ResizeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = WindowsApp::GetInst().GetHwnd();
	HINSTANCE hInst = WindowsApp::GetInst().GetHInstance();

	switch (message)
	{
	case WM_COMMAND:
	{
		int cmd = LOWORD(wParam);

		switch (cmd)
		{
		case IDOK:
			onResize(hDlg);
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	}

	return FALSE;
}

void WinProc::ResizeProc::onResize(HWND hwnd)
{
	std::wstring width = ReadDlg(hwnd, ID_EDIT1);
	std::wstring height = ReadDlg(hwnd, ID_EDIT2);

	WindowSize winSize = {};
	winSize.Height = std::stoi(height);
	winSize.Width = std::stoi(width);

	//리사이즈 완료
	WindowsApp::GetInst().ResizeWindow(winSize, false);

	WinUtile::Alert(hwnd, L"Resize Done.", L"Alert");

	EndDialog(hwnd, IDOK);
}
