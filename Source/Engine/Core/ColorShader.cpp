#include "ColorShader.h"

bool ColorShader::Initialize(ComPtr<ID3D11Device> device) {
	// 셰이더 로드
	_vs.CreateVertexShader(device, L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/VS.hlsl", "VS");
	_ps.CreatePixelShader(device, L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/PS.hlsl", "PS");

	// 상수 버퍼 생성
	if (!_constantBuffer.Create(device)) return false;
	if (!_lightBuffer.Create(device)) return false;

	return true;
}


void ColorShader::Render(ComPtr <ID3D11DeviceContext> context, const TransformData& tData, const LightData& lData) {
	// 1. 상수 버퍼 먼저 건드리기 (Update & Bind)
	_constantBuffer.Update(context, tData);
	_constantBuffer.BindVS(context, 0); // b0 슬롯

	_lightBuffer.Update(context, lData);
	_lightBuffer.BindPS(context, 1);

	// 2. 셰이더 들어가기 (Bind)
	_vs.Bind(context);
	_ps.Bind(context);
}
