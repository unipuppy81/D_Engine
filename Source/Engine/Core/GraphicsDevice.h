#pragma once
#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class GraphicsDevice {
public:
	// 생성자: 위도우 창의 번호 받아서 기억
	GraphicsDevice(HWND hwnd);
	~GraphicsDevice() = default;

	// 초기화 : 그래픽 카드와 통신할 준비
	void Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool windowScrren);
	void Shutdown();


	// 매 프레임 시작과 끝, Begin 에서 다 지우고 End 에서 보냄
	void RenderBegin(float red, float green, float blue, float alpha);
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetContext() { return _context; }
private:
	HWND _hwnd;
	bool _vsync_enabled = false;

	// --- DirectX 11 핵심 객체들 (ComPtr 사용으로 자동 메모리 관리) ---
	ComPtr<ID3D11Device>		_device;		// 리소스(Mesh, Texture 등)를 만드는 장치
	ComPtr<ID3D11DeviceContext>	_context;		// GPU에게 명령(그려라, 지워라)
	ComPtr<IDXGISwapChain>		_swapChain;		// 화면 교체해주는 장치

	ComPtr<ID3D11RenderTargetView>	_renderTargetView;	// 실제 그릴 도화지
	D3D11_VIEWPORT					_viewport = {};		// 도화지 중 어디에 그릴지 정하는 영역

	ComPtr<ID3D11Texture2D>         _depthStencilBuffer; // 실제 깊이 값이 저장될 텍스처
	ComPtr<ID3D11DepthStencilView>  _depthStencilView;   // 깊이 버퍼를 바라보는 뷰
};