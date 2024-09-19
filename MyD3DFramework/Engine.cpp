#include "pch.h"
#include "Engine.h"
#include "WindowsApp.h"
#include "GameTimer.h"
#include "Renderer.h"
#include "InputManager.h"

bool Engine::Init(HINSTANCE inHinst)
{
	//COM 라이브러리 초기화
	CHECK_FAILED(::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED));

	if (!WindowsApp::GetInst().Init(inHinst, L"My D3DX Program", L"My D3DX Program"))
	{
		return false;
	}

	if (!Renderer::GetInst().InitD3D11Device())
	{
		return false;
	}

	//DX 의존성 객체 생성
	Renderer::GetInst().CreateDeviceDependentResource();

	//윈도우 사이즈 설정
	WindowsApp::GetInst().ResizeWindow(
		WindowSize::s_defaultWindowSize,
		true,
		WindowPosition{100, 60}
		);

	//게임 타이머 초기화
	GameTimer::GetInst().Reset();

	//입력 장치 초기화
	InputManager::GetInst().Init();

	//리소스 초기화
	ResourceManager::GetInst().Init();
	ResourceManager::GetInst().LoadResource();

	//메모리 풀 생성
	WBuffer512::GetInst();

	//Input Assembler (IA)
	Renderer::GetInst().InputAssembler();

	return true;
}

void Engine::Update()
{
	//타이머 업데이트
	GameTimer& timer = GameTimer::GetInst();
	timer.Update();

	//InputManager 업데이트
	InputManager::GetInst().Update();
	DebugHandler();

	//렌더링 머신 업데이트
	Renderer::GetInst().Update(timer.GetDeltaTime());
}

void Engine::Render()
{
	Renderer& r = Renderer::GetInst();
	r.ClearBuffer(Colors::SlateGray);

	//성능측정
	r.Render();
	RenderDebugData();

	//스왑체인
	r.Present();
}

int Engine::Run()
{
	HWND hwnd = WindowsApp::GetInst().GetHwnd();
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!m_bAppPause)
			{
				Update();
				Render();
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	}

	Renderer::GetInst().CleanUp();
	CoUninitialize();

	return (int)msg.wParam;
}

void Engine::SetAppPauseState(bool trigger)
{
	if (trigger != m_bAppPause)
	{
		m_bAppPause = trigger;

		//타이머 설정
		GameTimer& timer = GameTimer::GetInst();

		if (m_bAppPause)
		{
			timer.TimerStop();
		}
		else
		{
			timer.TimerStart();
		}
	}
}

void Engine::DebugHandler()
{
}

void Engine::RenderDebugData()
{
	const static Vector2 initialPosition = { 10.f,10.f };
	WBuffer512& buffer = WBuffer512::GetInst();

	//성능측정
	GameTimer& t = GameTimer::GetInst();
	buffer.Input(L"Fps: %lld\nDelta Time: %.7f\n", t.GetFPS(), t.GetDeltaTime());

	//피쳐 레벨
	buffer.Append(L"Feature Level: " + Renderer::GetInst().GetFeatureLevelToString());

	Renderer::GetInst().DrawString(buffer.c_str(), initialPosition, eFont::Consolas_16pt, Colors::LightGreen);
}
