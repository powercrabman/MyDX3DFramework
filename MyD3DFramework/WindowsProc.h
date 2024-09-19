#pragma once

namespace WinProc
{
	std::wstring ReadDlg(HWND parentHwnd, int id);
	std::wstring ReadDlg(HWND dlgHwnd);

	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	namespace ResizeProc
	{
		LRESULT CALLBACK ResizeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		void onResize(HWND hwnd);
	}
}

