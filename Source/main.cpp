#include "Engine/Core/Window.h"
#include "Engine/Core/GraphicsDevice.h"
#include "Engine/Core/VertexBuffer.h"
#include "Engine/Core/IndexBuffer.h"
#include "Engine/Core/Shader.h"
#include "Engine/Core/ConstantBuffer.h"
#include "Engine/Core/RasterizerState.h"

#include <d3dcompiler.h>
#include <memory>
#include <vector>

// 정점 구조체
struct Vertex {
	float x, y, z;
	float r, g, b, a;
};

// 1. 상수 버퍼에 담을 구조체 정의 (HLSL과 순서가 같아야 함)
struct TransformData {
	float x, y, z;
	float padding; // 16바이트 정렬용
};

int main() {
	Window window;
	if (!window.Create(L"D_Engine", 1280, 720)) return -1;

	GraphicsDevice graphics(window.GetHWND());
	graphics.Init();


	ConstantBuffer<TransformData> cb;
	cb.Create(graphics.GetDevice());

	TransformData data = { 0.0f, 0.0f, 0.0f, 0.0f };

	RasterizerState rs;
	// 일반적인 설정: 면을 채우고(SOLID), 뒷면은 그리지 마라(BACK) -> while문 전에 실행하기만 하면 됨
	
	// D3D11_FILL_WIREFRAME(디버그용), D3D11_FILL_SOLID (기본값)
	// D3D11_CULL_BACK vs D3D11_CULL_NONE (컬링)
	rs.Create(graphics.GetDevice(), D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);


	// 1. 데이터 준비
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

	// 2. 버퍼 생성
	VertexBuffer vb;
	vb.Create(graphics.GetDevice(), vertices);

	IndexBuffer ib;
	ib.Create(graphics.GetDevice(), indices);

	// 3. 셰이더 생성

	/*
	* 기존 방식인데 Blob 는 기계어 덩어리 그거를 vertex, pixel 에 넣은것
	ComPtr<ID3DBlob> vsBlob, psBlob;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	*/
	// 셰이더 컴파일
	Shader shader;
	shader.Create(graphics.GetDevice(), L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/Default.hlsl", "VS", "PS");


	// 상수 버퍼용 데이터 2개 준비
	// 사각형 A: 앞 (Z = 0.0f)
	TransformData dataA = { -0.2f, 0.0f, 0.0f, 0.0f };
	// 사각형 B: 뒤 (Z = 0.5f) - 사각형 A와 살짝 겹치게 배치
	TransformData dataB = { 0.2f, 0.0f, 0.5f, 0.0f };


	// 4. 게임 루프
	while (window.Run()) {
		graphics.RenderBegin();
		auto context = graphics.GetContext();
		
		rs.Bind(context);

		data.x += 0.001f;
		if (data.x > 1.0f) data.x = -1.0f; // 화면 끝으로 가면 다시 왼쪽으로

		cb.Update(graphics.GetContext(), data); // GPU로 데이터 전송
		cb.Bind(graphics.GetContext(), 0);      // 0번 슬롯(b0)에 장착


		// 쉐이더 장착 (VS, PS, InputLayout이 한꺼번에 셋팅됩니다)
		shader.Bind(context);

		// 버퍼 장착
		vb.Bind(context);
		ib.Bind(context);

		// 토폴로지 설정 (삼각형으로 그리겠다)
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// --- 첫 번째 사각형 (A) 그리기 ---
		cb.Update(context, dataA);
		cb.Bind(context, 0);
		context->DrawIndexed(ib.GetCount(), 0, 0);

		// --- 두 번째 사각형 (B) 그리기 ---
		// 코드상으로는 나중에 그리지만, Z값이 더 크므로(0.5f) 
		// 깊이 버퍼가 정상이라면 사각형 A 뒤에 가려져야 합니다.
		cb.Update(context, dataB);
		cb.Bind(context, 0);
		context->DrawIndexed(ib.GetCount(), 0, 0);


		// 그리기 (인덱스 수만큼)
		context->DrawIndexed(ib.GetCount(), 0, 0);

		graphics.RenderEnd();
	}

	return 0;
}