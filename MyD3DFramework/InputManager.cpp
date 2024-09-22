#include "pch.h"
#include "InputManager.h"
#include "WindowsApp.h"

void InputManager::Init()
{
	std::fill(m_stateList.begin(), m_stateList.end(), eKeyState::Away);
	m_hwnd = WindowsApp::GetInst().GetHwnd();
}

void InputManager::Update()
{
	HWND activeWindow = GetActiveWindow();
	bool isActiveWindow = (activeWindow == m_hwnd);

	BYTE asciiKeys[UINT8_MAX + 1] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (uint32_t key = 0; key < UINT8_MAX; key++)
	{
		eKeyState& state = m_stateList[key];

		if (isActiveWindow)
		{
			if (asciiKeys[key] & 0x80)
			{
				state = (state == eKeyState::Press || state == eKeyState::Hold) ? eKeyState::Hold : eKeyState::Press;
			}
			else
			{
				state = (state == eKeyState::Press || state == eKeyState::Hold) ? eKeyState::Release : eKeyState::Away;
			}
		}
		else
		{
			state = (state == eKeyState::Hold || state == eKeyState::Press) ? eKeyState::Release : eKeyState::Away;
		}
	}

	// 마우스 위치 업데이트
	m_prevMousePos = m_mousePos;
	::GetCursorPos(&m_mousePos);
	::ScreenToClient(m_hwnd, &m_mousePos);
}

