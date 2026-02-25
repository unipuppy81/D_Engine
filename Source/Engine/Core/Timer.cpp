#include "Timer.h"

Timer::Timer()
    : m_secondsPerCount(0.0), m_deltaTime(-1.0), m_baseTime(0), m_prevTime(0), m_currTime(0)
{
    __int64 countsPerSec;
    // 하드웨어의 초당 카운트 빈도를 가져옴
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    m_secondsPerCount = 1.0 / (double)countsPerSec;

    // 시작 시간 초기화
    QueryPerformanceCounter((LARGE_INTEGER*)&m_baseTime);
    m_prevTime = m_baseTime;
}

void Timer::Tick() {
    QueryPerformanceCounter((LARGE_INTEGER*)&m_currTime);

    // 이전 프레임과 현재 프레임 사이의 시간 계산
    m_deltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;

    // 다음 프레임을 위해 현재 시간을 이전 시간으로 저장
    m_prevTime = m_currTime;

    // 음수 방지 (절전 모드나 일시적인 하드웨어 오류 대비)
    if (m_deltaTime < 0.0) {
        m_deltaTime = 0.0;
    }
}

void Timer::Reset() {
    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    m_baseTime = currTime; // 엔진 가동 시작 시점 갱신
    m_prevTime = currTime; // 이전 프레임 시점을 현재로 일치
    m_stopTime = 0;       
    m_isStopped = false;
}

float Timer::GetDeltaTime() const {
    return static_cast<float>(m_deltaTime);
}

float Timer::GetTotalTime() const {
    return static_cast<float>((m_currTime - m_baseTime) * m_secondsPerCount);
}