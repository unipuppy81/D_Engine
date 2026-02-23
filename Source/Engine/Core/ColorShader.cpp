#include "ColorShader.h"

bool ColorShader::Initialize(ComPtr<ID3D11Device> device) {
	// 셰이더 로드
	_vs.CreateVertexShader(device, L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/VS.hlsl", "VS");
	_ps.CreatePixelShader(device, L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/PS.hlsl", "PS");

	// 상수 버퍼 생성
	if (!_constantBuffer.Create(device)) {
		return false;
	}

	return true;
}


void ColorShader::Render(ComPtr <ID3D11DeviceContext> context, const TransformData& data) {
	// 1. 상수 버퍼 먼저 건드리기 (Update & Bind)
	_constantBuffer.Update(context, data);
	_constantBuffer.Bind(context, 0); // b0 슬롯

	// 2. 셰이더 들어가기 (Bind)
	_vs.Bind(context);
	_ps.Bind(context);
}
