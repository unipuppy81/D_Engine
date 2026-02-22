#include "GraphicsDevice.h"

// 생성자: 윈도우 핸들을 멤버 변수에 저장
GraphicsDevice::GraphicsDevice(HWND hwnd) : _hwnd(hwnd) {}

void GraphicsDevice::Init() {
	// 1. SwapChain 설정
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;							// 백버퍼 개수
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 색상 포맷(RGBA 8비트)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 이 버퍼를 도화지로 쓴다
	sd.OutputWindow = _hwnd;					// 그림 출력할 윈도우 창
	sd.SampleDesc.Count = 1;					// 안티에일리어싱(계단현상 방지) 꺼둠
	sd.Windowed = TRUE;							// 창모드

	// 2. 그래픽 카드(Device), Context, SwapChian 생성
	D3D11CreateDeviceAndSwapChain(
		nullptr,						// 기본 그래픽 카드 사용
		D3D_DRIVER_TYPE_HARDWARE,		// 그래픽 카드 하드웨어 가속 사용
		nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION,				// 현재 사용하는 SDK 버전
		&sd,							// SwapChain 정보
		&_swapChain,					// 생성된 SwapChain 여기에 저장
		&_device,						// 생성된 Device 여기에 저장
		nullptr,
		&_context						// 생성된 Context 여기에 저장
	);


	// 3. RenderTargetView 생성
	// SwapChain이 가지고 있는 0번 버퍼(백버퍼)를 텍스트 형식으로 가져옴
	ComPtr<ID3D11Texture2D> backBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());

	// 가져온 텍스처를 "그림 그릴 수 있는 도화지(RTV)" 형태로 변환하여 저장
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &_renderTargetView);


	// 4. Viewport 설정: 창의 어느 영역에 그림 채울지 결정
	RECT rc;
	GetClientRect(_hwnd, &rc);		// 현재 윈도우 창의 크기
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.Width = static_cast<float>(rc.right - rc.left);
	_viewport.Height = static_cast<float>(rc.bottom - rc.top);
	_viewport.MaxDepth = 1.0f;
	_viewport.MinDepth = 0.0f;
}

void GraphicsDevice::RenderBegin()
{
	float clearColor[4] = { 0.1f, 0.15f, 0.3f, 1.0f };

	// 지휘관에게 도화지를 깨끗하게 닦으라고 명령
	_context->ClearRenderTargetView(_renderTargetView.Get(), clearColor);

	// 지휘관에게 "어떤 도화지에(RTV)", "어떤 영역(Viewport)"에 그릴지 알려줌
	_context->RSSetViewports(1, &_viewport);
	_context->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
}

void GraphicsDevice::RenderEnd() {
	// 5. Present: 뒤에서 몰래 그린 그림을 실제 화면(전광판)으로 '짜잔' 하고 교체
	// 1은 VSync(수직동기화)를 켜서 화면 찢어짐을 방지함
	_swapChain->Present(1, 0);
}