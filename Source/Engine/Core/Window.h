#pragma once
#include <Windows.h>
#include <string>

class Window {
public:
	// 창 이름과 가로/세로 크기 받아 초기화
	bool Create(const std::wstring& title, int width, int height);

	// 윈도우 메시지 처리 (종료 여부 반환)
	bool Run();

	HWND GetHWND() const { return _hwnd; }

private:
	// 윈도우 메시지 받는 콜백 함수
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMSG, WPARAM wParam, LPARAM lParam);
	HWND _hwnd = nullptr;
	HINSTANCE _hInst = nullptr;
};