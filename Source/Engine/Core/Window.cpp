#include "Window.h"

// 윈도우 메시지 처리기( 마우스 클릭, 창 닫기 등 이벤트 처리)
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


bool Window::Create(const std::wstring& title, int width, int height) {
	_hInst = GetModuleHandle(nullptr);

	// 1. 윈도우 클래스 등록
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = _hInst;
	wc.lpszClassName = L"D_Engine_WindowClass";
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClass(&wc);

	// 2. 창 크기 계산 (타이틀 바 제외)
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// 3. 윈도우 생성
	_hwnd = CreateWindowEx(
		0, L"D_Engine_WindowClass", title.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, _hInst, nullptr
	);

	if (!_hwnd) return false;

	ShowWindow(_hwnd, SW_SHOW);
	UpdateWindow(_hwnd);

	return true;
}


bool Window::Run() {
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}