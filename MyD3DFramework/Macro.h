#pragma once

#define ASSERT(cond, hint) assert(cond)

#define CHECK_FAILED(hr) 									   										\
	if (FAILED(hr))																					\
	{																								\
		auto& buffer = Buffer512W::GetInst();														\
		buffer.Input(L"DirectX Fail!\nError Code: %d", static_cast<int>(hr));						\
		WinUtile::Alert(buffer.c_str(), L"Error");													\
		PostQuitMessage(0);																			\
	}

#define CHECK_FAILED_MESSAGE(hr, msg) 									   							\
	if (FAILED(hr))																					\
	{																								\
		auto& buffer = Buffer512W::GetInst();													    \
		buffer.Input(L"DirectX Fail!\nError Code: %ld\nMessage: %s", static_cast<HRESULT>(hr), msg);		\
		WinUtile::Alert(buffer.c_str(), L"Error");													\
		PostQuitMessage(0);																			\
	}

#define VERTIFY(cond, hint)														   \
if (!cond)																		   \
{																				   \
	WinUtile::Alert(hint, L"Error");											   \
	PostQuitMessage(0);															   \
}

#define DEFINE_SINGLE(classname)   \
public:							   \
inline static classname& GetInst() \
{								   \
	static classname inst;		   \
	return inst;				   \
}								   

#define KEY_HOLD(keycode) InputManager::GetInst().GetKeyHold(keycode)
#define KEY_RELEASE(keycode) InputManager::GetInst().GetKeyRelease(keycode)
#define KEY_PRESS(keycode) InputManager::GetInst().GetKeyPress(keycode)
#define KEY_AWAY(keycode) InputManager::GetInst().GetKeyAway(keycode)
#define KEY_DOWN(keycode) InputManager::GetInst().GetKeyDown(keycode)
#define KEY_UP(keycode) InputManager::GetInst().GetKeyUp(keycode)

#define NODISCARD [[nodiscard]]

#define USING_SUPER(classname)		using Super = classname
#define DELTA_TIME GameTimer::GetInst().GetDeltaTime()

#ifdef _DEBUG && (true)
#define TODO(msg) static_assert(false, msg)
#else
#define TODO(mas)
#endif

