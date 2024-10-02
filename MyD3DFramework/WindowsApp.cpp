#include "pch.h"
#include "resource.h"
#include "WindowsApp.h"
#include "WindowsProc.h"
#include "Engine.h"
#include "Renderer.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

bool WindowsApp::Initialize(HINSTANCE inHinst, std::wstring_view titleName, std::wstring_view className)
{
	m_titleName = titleName;
	m_windowClass = className;
	m_hinst = inHinst;

	WNDCLASSEX wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WinProc::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hinst;
	wcex.hIcon = LoadIcon(m_hinst, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_windowClass.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	ATOM hr = RegisterClassExW(&wcex);

	//초기화 실패
	if (hr == 0)
	{
		return false;
	}

	m_hwnd = CreateWindowW(
		m_windowClass.c_str(),
		m_titleName.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		nullptr,
		nullptr,
		inHinst,
		nullptr
	);

	if (!m_hwnd)
	{
		DWORD error = GetLastError();
		return false;
	}

	return true;
}

void WindowsApp::SetFullScreen(bool trigger)
{
	if (trigger)
	{
		if (!m_bFullScreen)
		{
			m_bFullScreen = true;

			Renderer::GetInst().SetFullScreen(true);

			RECT winRect = {};
			GetClientRect(m_hwnd, &winRect);
			ResizeWindow(winRect);
		}
	}
	else
	{
		if (m_bFullScreen)
		{
			m_bFullScreen = false;

			Renderer::GetInst().SetFullScreen(false);

			ResizeWindow(WindowSize::s_defaultWindowSize);
		}
	}
}

void WindowsApp::ResizeWindow(const WindowSize& inWinSize,bool setWinPosFlag , const WindowPosition& inWinPos)
{
	RECT winRect = {};
	winRect.right = static_cast<LONG>(inWinSize.Width);
	winRect.bottom = static_cast<LONG>(inWinSize.Height);

	m_windowSize.Width = inWinSize.Width;
	m_windowSize.Height = inWinSize.Height;

	::AdjustWindowRect(&winRect, GetWindowLong(m_hwnd, GWL_STYLE), false);

	if (setWinPosFlag)
	{
		::SetWindowPos(m_hwnd, NULL,
			inWinPos.X, inWinPos.Y,
			winRect.right - winRect.left,
			winRect.bottom - winRect.top,
			SWP_NOZORDER | SWP_NOREDRAW | SWP_SHOWWINDOW);
	}
	else
	{
		::SetWindowPos(m_hwnd, NULL,
			0, 0,
			winRect.right - winRect.left,
			winRect.bottom - winRect.top,
			SWP_NOZORDER | SWP_NOREDRAW | SWP_SHOWWINDOW | SWP_NOMOVE);
	}

	if (Renderer::GetInst().IsInitalized())
	{
		Renderer::GetInst().ResizeWindow(inWinSize);
	}
}

