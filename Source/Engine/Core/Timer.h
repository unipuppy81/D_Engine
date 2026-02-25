#pragma once
#include <windows.h>

class Timer {
public:
    Timer();
    ~Timer() = default;

    void Tick();                // 매 프레임 호출하여 시간 갱신
    void Reset();
    float GetDeltaTime() const; // 프레임 사이의 시간 간격 (초 단위)
    float GetTotalTime() const; // 엔진 실행 후 누적 시간


private:
    double m_secondsPerCount;   // 카운트 1당 초 단위 시간
    double m_deltaTime;         // 프레임 간 시간

    __int64 m_baseTime;         // 엔진 시작 시간
    __int64 m_prevTime;         // 이전 프레임 시간
    __int64 m_currTime;         // 현재 프레임 시간

    __int64 m_stopTime;         // 일시정지 누른 시간
    bool m_isStopped;           // 엔진 시간이 흐르고 있는지 멈췄는지 나타내는 플래그
};