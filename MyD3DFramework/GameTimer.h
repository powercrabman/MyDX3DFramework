#pragma once

class GameTimer
{
public:
	DEFINE_SINGLE(GameTimer);

	void Update();
	
	void Reset();
	void TimerStop();
	void TimerStart();

	float GetDeltaTime() const;
	float GetTotalTime() const;
	uint64 GetFPS() const;

private:
	GameTimer();
	~GameTimer() = default;

private:
	uint64 m_prevCount = 0; //이전 시간

	uint64 m_stopTime = 0;  //중지 시작시간
	uint64 m_pauseTime = 0; //총 중지 시간
	uint64 m_baseTime = 0;  //타이머 시작시간

	float m_deltaTime = 0.f;	//단위 시간
	float m_countPeriod = 0.f;  //클락 진동수 역수

	float m_accTime = 0.f; //시간 누적 변수
	uint64 m_frameCount = 0; //프레임 측정 변수
	uint64 m_fps = 0; //fps

	bool m_bStop = false; //타이머 중단 플래그
};

inline GameTimer::GameTimer()
{
	//주기 초기화
	LARGE_INTEGER freq = {};
	::QueryPerformanceFrequency(&freq);

	m_countPeriod = 1.f / static_cast<float>(freq.QuadPart);
	Reset();
	
}

inline void GameTimer::Update()
{
	//타이머 일시중지 시 예외 처리
	if (m_bStop)
	{
		m_deltaTime = 0.f;
		return;
	}

	LARGE_INTEGER curCount = {};
	::QueryPerformanceCounter(&curCount);

	m_deltaTime = (curCount.QuadPart - m_prevCount) * m_countPeriod;
	m_prevCount = curCount.QuadPart;
	m_accTime += m_deltaTime;
	++m_frameCount;

	if (m_accTime >= 1.f)
	{
		m_accTime -= 1.f;
		m_fps = m_frameCount;
		m_frameCount = 0;
	}
}

inline void GameTimer::Reset()
{
	//타이머 리셋
	LARGE_INTEGER curCount = {};
	::QueryPerformanceCounter(&curCount);

	m_prevCount = curCount.QuadPart;
	m_baseTime = curCount.QuadPart;

	m_stopTime = 0;
	m_pauseTime = 0;

	m_deltaTime = 0.f;

	m_bStop = false;
}

inline void GameTimer::TimerStop()
{
	//타이머 정지 함수
	if (!m_bStop)
	{
		LARGE_INTEGER curCount = {};
		::QueryPerformanceCounter(&curCount);

		m_deltaTime = 0.f;
		m_stopTime = curCount.QuadPart;
		m_bStop = true;
	}
}

inline void GameTimer::TimerStart()
{
	//정지된 타이머 재개 함수
	if (m_bStop)
	{
		LARGE_INTEGER curCount = {};
		::QueryPerformanceCounter(&curCount);

		m_pauseTime += curCount.QuadPart - m_stopTime;
		m_prevCount = curCount.QuadPart;
		m_stopTime = 0;
		m_bStop = false;
	}
}

inline float GameTimer::GetDeltaTime() const
{
	return m_deltaTime;
}

inline float GameTimer::GetTotalTime() const
{
	if (m_bStop)
	{
		return (m_stopTime - m_pauseTime - m_baseTime) * m_countPeriod;
	}
	else
	{
		LARGE_INTEGER curCount = {};
		::QueryPerformanceCounter(&curCount);

		return (curCount.QuadPart - m_pauseTime - m_baseTime) * m_countPeriod;
	}
}

inline uint64 GameTimer::GetFPS() const
{
	return m_fps;
}
