#include<iostream>
#include <d3d11.h>

int main() {
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	D3D_FEATURE_LEVEL featureLevel;

	// DX11 장치 생성 시도
	HRESULT hr = D3D11CreateDevice(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION, &device, &featureLevel, &context
	);

	if (SUCCEEDED(hr)) {
		std::cout << "DirectX 11 연동 성공" << std::endl;

		// 메모리 해제
		if (context) context->Release();
		if (device) device->Release();
	}
	else {
		std::cout << "연동 실패" << std::endl;
	}

	return 0;
}