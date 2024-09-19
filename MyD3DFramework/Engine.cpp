#include "pch.h"
#include "Engine.h"
#include "WindowsApp.h"
#include "GameTimer.h"
#include "Renderer.h"
#include "InputManager.h"

bool Engine::Init(HINSTANCE inHinst)
{
	//COM ���̺귯�� �ʱ�ȭ
	CHECK_FAILED(::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED));

	if (!WindowsApp::GetInst().Init(inHinst, L"My D3DX Program", L"My D3DX Program"))
	{
		return false;
	}

	if (!Renderer::GetInst().InitD3D11Device())
	{
		return false;
	}

	//DX ������ ��ü ����
	Renderer::GetInst().CreateDeviceDependentResource();

	//������ ������ ����
	WindowsApp::GetInst().ResizeWindow(
		WindowSize::s_defaultWindowSize,
		true,
		WindowPosition{100, 60}
		);

	//���� Ÿ�̸� �ʱ�ȭ
	GameTimer::GetInst().Reset();

	//�Է� ��ġ �ʱ�ȭ
	InputManager::GetInst().Init();

	//���ҽ� �ʱ�ȭ
	ResourceManager::GetInst().Init();
	ResourceManager::GetInst().LoadResource();

	//�޸� Ǯ ����
	WBuffer512::GetInst();

	//Input Assembler (IA)
	Renderer::GetInst().InputAssembler();

	return true;
}

void Engine::Update()
{
	//Ÿ�̸� ������Ʈ
	GameTimer& timer = GameTimer::GetInst();
	timer.Update();

	//InputManager ������Ʈ
	InputManager::GetInst().Update();
	DebugHandler();

	//������ �ӽ� ������Ʈ
	Renderer::GetInst().Update(timer.GetDeltaTime());
}

void Engine::Render()
{
	Renderer& r = Renderer::GetInst();
	r.ClearBuffer(Colors::SlateGray);

	//��������
	r.Render();
	RenderDebugData();

	//����ü��
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

		//Ÿ�̸� ����
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

	//��������
	GameTimer& t = GameTimer::GetInst();
	buffer.Input(L"Fps: %lld\nDelta Time: %.7f\n", t.GetFPS(), t.GetDeltaTime());

	//���� ����
	buffer.Append(L"Feature Level: " + Renderer::GetInst().GetFeatureLevelToString());

	Renderer::GetInst().DrawString(buffer.c_str(), initialPosition, eFont::Consolas_16pt, Colors::LightGreen);
}
