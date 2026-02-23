#include "GraphicsDevice.h"

// 생성자: 윈도우 핸들을 멤버 변수에 저장
GraphicsDevice::GraphicsDevice(HWND hwnd) : _hwnd(hwnd) {}

void GraphicsDevice::Init(int w, int h, bool vsync, HWND hwnd, bool fullScrren) {
	_vsync_enabled = vsync;
	
	// 1. SwapChain 설정
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;							// 백버퍼 개수
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 색상 포맷(RGBA 8비트)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 이 버퍼를 도화지로 쓴다
	sd.OutputWindow = _hwnd;					// 그림 출력할 윈도우 창
	sd.SampleDesc.Count = 1;					// 안티에일리어싱(계단현상 방지) 꺼둠
	sd.BufferDesc.Width = w;
	sd.BufferDesc.Height = h;
	sd.Windowed = fullScrren;					// 창모드



	// 2. Device & SwapChian 생성

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
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


	// 4. Viewport 설정
	RECT rc;
	GetClientRect(_hwnd, &rc);		// 현재 윈도우 창의 크기
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.Width = static_cast<float>(w);
	_viewport.Height = static_cast<float>(h);
	_viewport.MaxDepth = 1.0f;
	_viewport.MinDepth = 0.0f;


	// 5. DepthStencilView 생성
	D3D11_TEXTURE2D_DESC dsd = {};
	dsd.Width = static_cast<UINT>(rc.right - rc.left);
	dsd.Height = static_cast<UINT>(rc.bottom - rc.top);
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 24비트는 깊이(D), 8비트는 스텐실(S)
	dsd.SampleDesc.Count = 1;
	dsd.SampleDesc.Quality = 0;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// 실제 텍스처 생성
	_device->CreateTexture2D(&dsd, nullptr, &_depthStencilBuffer);
	// 텍스처를 DSV 형태로 생성
	_device->CreateDepthStencilView(_depthStencilBuffer.Get(), nullptr, &_depthStencilView);


	// 6. 생성된 뷰들을 GPU 파이프라인(Output Merger)에 장착
	_context->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

	// 7. 뷰포트도 이때 한 번 장착해두면 편리합니다.
	_context->RSSetViewports(1, &_viewport);
}

void GraphicsDevice::Shutdown() {
	// 전체화면은 종료 전 해제
	if (_swapChain) {
		_swapChain->SetFullscreenState(FALSE, nullptr);
	}

	// ComPtr이므로 nullptr 대입 시 참조 횟수가 감소하며 해제됩니다.
	_renderTargetView.Reset();
	_depthStencilView.Reset();
	_depthStencilBuffer.Reset();
	_swapChain.Reset();
	_context.Reset();
	_device.Reset();
}

void GraphicsDevice::RenderBegin(float r, float g, float b, float a)
{
	float color[4] = { r, g, b, a };

	// 1. RenderTarget 초기화
	_context->ClearRenderTargetView(_renderTargetView.Get(), color);

	// 2. DepthStencilView 초기화 (가장 먼 거리인 1.0f로 초기화)
	_context->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 3. 뷰포트 설정
	_context->RSSetViewports(1, &_viewport);

	// 4. 도화지와 깊이 기록부 동시에 연결 (마지막 인자가 nullptr에서 _depthStencilView로 변경)
	_context->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
}

void GraphicsDevice::RenderEnd() {
	// 5. Present: 뒤에서 몰래 그린 그림을 실제 화면(전광판)으로 '짜잔' 하고 교체
	// 1은 VSync(수직동기화)를 켜서 화면 찢어짐을 방지함
	_swapChain->Present(1, 0);
}