#pragma once

class Engine final
{
public:
	DEFINE_SINGLE(Engine);

	bool Initialize(HINSTANCE inHInst);
	int Run();

	void SetAppPauseState(bool trigger);
	void DebugHandler();
	void RenderDebugData();

private:
	Engine() = default;
	~Engine() = default;

	void Update();
	void Render();

private:
	bool m_bAppPause = false;
};
