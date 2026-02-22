#include "Engine/Core/Window.h"
#include "Engine/Core/GraphicsDevice.h"
#include "Engine/Core/VertexBuffer.h"
#include "Engine/Core/IndexBuffer.h"

#include <d3dcompiler.h>
#include <memory>
#include <vector>

// 정점 구조체
struct Vertex {
	float x, y, z;
	float r, g, b, a;
};

int main() {
	Window window;
	// 윈도우 창 생성
	if (!window.Create(L"D_Engine", 1280, 720)) return -1;

	// GrphicsDevice 초기화
	GraphicsDevice graphics(window.GetHWND());
	graphics.Init();

	// ---------------------------------------------------------
	// 1. 데이터 준비 (std::vector로 관리하면 클래스에 넣기 편합니다)
	// ---------------------------------------------------------
	std::vector<Vertex> vertices = {
		{ -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // 0: 왼쪽 위
		{  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // 1: 오른쪽 위
		{  0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }, // 2: 오른쪽 아래
		{ -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f }  // 3: 왼쪽 아래
	};

	std::vector<uint32_t> indices = {
		0, 1, 2,
		0, 2, 3
	};

	// ---------------------------------------------------------
	// 2. 버퍼 생성
	// ---------------------------------------------------------
	VertexBuffer vb;
	vb.Create(graphics.GetDevice(), vertices);

	IndexBuffer ib;
	ib.Create(graphics.GetDevice(), indices);

	// ---------------------------------------------------------
	// 3. 셰이더 및 레이아웃 (이 부분도 나중에 클래스로 뺄 예정입니다)
	// ---------------------------------------------------------
	ComPtr<ID3DBlob> vsBlob, psBlob;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;

	// 셰이더 컴파일
	D3DCompileFromFile(L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/Default.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &vsBlob, nullptr);
	graphics.GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);

	D3DCompileFromFile(L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/Default.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &psBlob, nullptr);
	graphics.GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);

	// 입력 레이아웃
	ComPtr<ID3D11InputLayout> inputLayout;
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	graphics.GetDevice()->CreateInputLayout(layoutDesc, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);


	// ---------------------------------------------------------
	// 4. 게임 루프
	// ---------------------------------------------------------
	while (window.Run()) {
		graphics.RenderBegin();

		auto context = graphics.GetContext();
		
		// 파이프라인 설정
		context->IASetInputLayout(inputLayout.Get());
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// ★ 클래스 Bind 함수 호출로 한 줄 정리!
		vb.Bind(context);
		ib.Bind(context);

		context->VSSetShader(vertexShader.Get(), nullptr, 0);
		context->PSSetShader(pixelShader.Get(), nullptr, 0);

		// 그리기
		context->DrawIndexed(ib.GetCount(), 0, 0);
		
		graphics.RenderEnd();
	}

	return 0;
}