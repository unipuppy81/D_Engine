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
	void Init();

	// 렌더링 시작: 화면 지우고 그릴 준비
	void RenderBegin();

	// 렌더링 종료: 그린 그림 실제 모니터로 보냄
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetContext() { return _context; }
private:
	HWND _hwnd;

	// --- DirectX 11 핵심 객체들 (ComPtr 사용으로 자동 메모리 관리) ---
	ComPtr<ID3D11Device>		_device;		// 리소스(Mesh, Texture 등)를 만드는 장치
	ComPtr<ID3D11DeviceContext>	_context;		// GPU에게 명령(그려라, 지워라)
	ComPtr<IDXGISwapChain>		_swapChain;		// 화면 교체해주는 장치

	ComPtr<ID3D11RenderTargetView>	_renderTargetView;	// 실제 그릴 도화지
	D3D11_VIEWPORT					_viewport = {};		// 도화지 중 어디에 그릴지 정하는 영역
};