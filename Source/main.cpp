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

// 상수 버퍼에 담을 구조체 정의 (HLSL과 순서가 같아야 함)
struct TransformData {
	float x, y, z;
	float padding; // 16바이트 정렬용
};

int main() {
	// 0. 윈도우 창 준비
	Window window;
	if (!window.Create(L"D_Engine", 1280, 720)) return -1;

	GraphicsDevice graphics(window.GetHWND());
	graphics.Init();

	// 1. 데이터 준비 (CPU Memory)
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
	TransformData data = { 0.0f, 0.0f, 0.0f, 0.0f };

	// 상수 버퍼용 데이터 2개 준비
	// 사각형 A: 앞 (Z = 0.0f)
	TransformData dataA = { -0.2f, 0.0f, 0.0f, 0.0f };
	// 사각형 B: 뒤 (Z = 0.5f) - 사각형 A와 살짝 겹치게 배치
	TransformData dataB = { 0.2f, 0.0f, 0.5f, 0.0f };

	// 2. GPU 버퍼 생성 (GPU Memory)
	VertexBuffer vb;
	vb.Create(graphics.GetDevice(), vertices);

	IndexBuffer ib;
	ib.Create(graphics.GetDevice(), indices);

	ConstantBuffer<TransformData> cb;
	cb.Create(graphics.GetDevice());



	// 3. 상태 및 쉐이더 생성
	//Shader shader; shader.Create(graphics.GetDevice(), L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/Default.hlsl", "VS", "PS");
	Shader vs, ps;
	vs.CreateVertexShader(graphics.GetDevice(), L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/VS.hlsl", "VS");
	ps.CreatePixelShader(graphics.GetDevice(), L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/PS.hlsl", "PS");

	// 일반적인 설정: 면을 채우고(SOLID), 뒷면은 그리지 마라(BACK)
	// D3D11_FILL_WIREFRAME(디버그용), D3D11_FILL_SOLID (기본값)
	// D3D11_CULL_BACK vs D3D11_CULL_NONE (컬링)
	RasterizerState rs;
	rs.Create(graphics.GetDevice(), D3D11_FILL_SOLID, D3D11_CULL_NONE);


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
		vs.Bind(context);
		ps.Bind(context);

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

		// 그리기 (인덱스 수만큼)
		context->DrawIndexed(ib.GetCount(), 0, 0);

		graphics.RenderEnd();
	}

	return 0;
}