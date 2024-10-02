#pragma once

class WindowsApp final
{
private:
	WindowsApp() = default;
	~WindowsApp() = default;

public:
	DEFINE_SINGLE(WindowsApp);

	bool Initialize(
		HINSTANCE inHinst,
		std::wstring_view titleName,
		std::wstring_view className
	);

	//윈도우 사이즈 제어
	void SetFullScreen(bool trigger);
	void ResizeWindow(const WindowSize& inWinSize,
		bool setWinPosFlag = false,
		const WindowPosition& inWinPos = WindowPosition::s_defaultWindowPosition);

	HINSTANCE GetHInstance() const { return m_hinst; }
	HWND GetHwnd() const { return m_hwnd; }

	WindowSize GetWindowSize() const { return m_windowSize; }
	float GetAspectRatio() const { return static_cast<float>(m_windowSize.Width) / m_windowSize.Height; }

private:
	std::wstring m_titleName = {};
	std::wstring m_windowClass = {};

	HINSTANCE m_hinst = NULL;
	HWND m_hwnd = NULL;

	WindowSize m_windowSize = {};

	bool m_bFullScreen = false;
};

