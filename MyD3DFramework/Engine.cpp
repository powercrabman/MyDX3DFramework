#include "pch.h"
#include "Engine.h"
#include "WindowsApp.h"
#include "GameTimer.h"
#include "Renderer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "DevScene.h"

bool Engine::Initialize(HINSTANCE inHinst)
{
	//COM 라이브러리 초기화
	CHECK_FAILED(::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED));

	if (!WindowsApp::GetInst().Initialize(inHinst, L"My D3DX Program", L"My D3DX Program"))
	{
		return false;
	}

	if (!Renderer::GetInst().InitD3D11Device())
	{
		return false;
	}

	//DX 의존성 객체 생성
	Renderer::GetInst().InitializeDirectXTKResource();

	//윈도우 사이즈 설정
	WindowsApp::GetInst().ResizeWindow(
		WindowSize::s_defaultWindowSize,
		true,
		WindowPosition{100, 60}
		);

	//게임 타이머 초기화
	GameTimer::GetInst().Reset();

	//입력 장치 초기화
	InputManager::GetInst().Initialize();

	//리소스 초기화
	ResourceManager::GetInst().Initialize();
	ResourceManager::GetInst().LoadResource();

	//메모리 풀 생성
	Buffer512W::GetInst();

	//렌더링 리소스 생성
	Renderer::GetInst().InitializeRenderResoucre();

	//씬 관리
	SceneManager::GetInst().InitializeScene();
	SceneManager::GetInst().SwitchScene<DevScene>(); //씬 설정 

	return true;
}

void Engine::Update()
{
	//타이머 업데이트
	GameTimer& timer = GameTimer::GetInst();
	timer.Update();

	//InputManager 업데이트
	InputManager::GetInst().Update();
	//DebugHandler(); // 지금 사용하지 않음

	//Scene 업데이트
	SceneManager::GetInst().GetCurrentScene()->UpdateSceneCore();
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
	Buffer512W& buffer = Buffer512W::GetInst();

	//성능측정
	GameTimer& t = GameTimer::GetInst();
	buffer.Input(L"Fps: %lld\nDelta Time: %.7f\n", t.GetFPS(), t.GetDeltaTime());

	//피쳐 레벨
	buffer.Append(L"Feature Level: %s", Renderer::GetInst().GetFeatureLevelToString());

	Renderer::GetInst().DrawString(buffer.c_str(), initialPosition, eFont::Consolas_16pt, Colors::LightGreen);
}
