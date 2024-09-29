#include "pch.h"
#include "framework.h"
#include "Engine.h"
#include "WindowsApp.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Engine& engine = Engine::GetInst();

    if (!engine.Initialize(hInstance))
    {
        WinUtile::Alert(L"프로세스 초기화에 실패했습니다.", L"Error");
        return -1;
    }

    return engine.Run();
}
