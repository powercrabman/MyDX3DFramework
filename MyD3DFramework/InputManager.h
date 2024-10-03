#pragma once

enum class eKeyCode
{
	LB_Mouse = VK_LBUTTON,
	RB_Mouse = VK_RBUTTON,
	MB_Mouse = VK_MBUTTON,

	Left = VK_LEFT,
	Right = VK_RIGHT,
	Up = VK_UP,
	Down = VK_DOWN,

	Enter = VK_RETURN,
	L_Alt = VK_LMENU,
	L_Ctrl = VK_LCONTROL,
	L_Shift = VK_LSHIFT,
	Space = VK_SPACE,
	TAB = VK_TAB,
	ESC = VK_ESCAPE,

	A = 'A',
	B = 'B',
	C = 'C',
	D = 'D',
	E = 'E',
	F = 'F',
	G = 'G',
	H = 'H',
	I = 'I',
	J = 'J',
	K = 'K',
	L = 'L',
	M = 'M',
	N = 'N',
	O = 'O',
	P = 'P',
	Q = 'Q',
	R = 'R',
	S = 'S',
	T = 'T',
	U = 'U',
	V = 'V',
	W = 'W',
	X = 'X',
	Y = 'Y',
	Z = 'Z',

	NUMPAD_1 = VK_NUMPAD1,
	NUMPAD_2 = VK_NUMPAD2,
	NUMPAD_3 = VK_NUMPAD3,
	NUMPAD_4 = VK_NUMPAD4,
	NUMPAD_5 = VK_NUMPAD5,
	NUMPAD_6 = VK_NUMPAD6,
	NUMPAD_7 = VK_NUMPAD7,
	NUMPAD_8 = VK_NUMPAD8,
	NUMPAD_9 = VK_NUMPAD9,

	F9 = VK_F9,
	F8 = VK_F8,
	F7 = VK_F7,
	F6 = VK_F6,
	F5 = VK_F5,
	F4 = VK_F4,
	F3 = VK_F3,
	F2 = VK_F2,
	F1 = VK_F1,

	Count
};

enum class eKeyState
{
	Press,
	Hold,
	Release,
	Away
};

class InputManager
{
	DEFINE_SINGLE(InputManager)
private:
	InputManager() = default;
	virtual ~InputManager() = default;

public:
	void Initialize();
	void Update();

	bool GetKeyHold(eKeyCode key) { return m_stateList[static_cast<uint8>(key)] == eKeyState::Hold; }
	bool GetKeyPress(eKeyCode key) { return m_stateList[static_cast<uint8>(key)] == eKeyState::Press; }
	bool GetKeyRelease(eKeyCode key) { return m_stateList[static_cast<uint8>(key)] == eKeyState::Release; }
	bool GetKeyAway(eKeyCode key) { return m_stateList[static_cast<uint8>(key)] == eKeyState::Away; }

	bool GetKeyDown(eKeyCode key) { return GetKeyHold(key) && GetKeyPress(key); }
	bool GetKeyUp(eKeyCode key) { return GetKeyRelease(key) && GetKeyAway(key); }

	POINT GetMousePos()const { return m_mousePos; }
	POINT GetDeltaMousePos() const { return POINT{ m_mousePos.x - m_prevMousePos.x, m_mousePos.y - m_prevMousePos.y }; }

private:
	std::array<eKeyState, UINT8_MAX> m_stateList = {};
	POINT m_mousePos = {};	POINT m_prevMousePos = {};

	HWND m_hwnd = NULL;
};

