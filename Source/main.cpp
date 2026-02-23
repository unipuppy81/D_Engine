#include "Engine/Core/Window.h"
#include "Engine/Core/GraphicsDevice.h"
#include "Engine/Core/VertexBuffer.h"
#include "Engine/Core/IndexBuffer.h"
#include "Engine/Core/Shader.h"
#include "Engine/Core/ConstantBuffer.h"
#include "Engine/Core/RasterizerState.h"
#include "Engine/Core/Model.h"

#include <d3dcompiler.h>
#include <memory>
#include <vector>



// 상수 버퍼에 담을 구조체 정의 (HLSL과 순서가 같아야 함)
struct TransformData {
	float x, y, z;
	float padding; // 16바이트 정렬용
};

int main() {
	// 윈도우 창 준비
	Window window;
	if (!window.Create(L"D_Engine", 1280, 720)) return -1;

	GraphicsDevice graphics(window.GetHWND());
	graphics.Init(1280, 720, true, window.GetHWND(), false);

	// 데이터 준비 및 버퍼 생성을 Model 클래스가 담당
	Model* model = new Model();
	if (!model->Initialize(graphics.GetDevice())) return -1;

	// 상수 버퍼 및 셰이더 (아직 클래스화 전이라 유지)
	TransformData dataA = { -0.2f, 0.0f, 0.0f, 0.0f };
	TransformData dataB = { 0.2f, 0.0f, 0.5f, 0.0f };

	ConstantBuffer<TransformData> cb;
	cb.Create(graphics.GetDevice());

	// 상태 및 쉐이더 생성
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
		graphics.RenderBegin(0.1f, 0.1f, 0.1f, 1.0f);
		auto context = graphics.GetContext();
		
		// 셰이더 장착
		vs.Bind(context);
		ps.Bind(context);
		rs.Bind(context);

		// 모델 장착 (내부에서 vb, ib, topology 셋팅)
		model->Render(context);

		// --- 첫 번째 사각형 (A) 그리기 ---
		cb.Update(context, dataA);
		cb.Bind(context, 0);
		context->DrawIndexed(model->GetIndexCount(), 0, 0);

		// --- 두 번째 사각형 (B) 그리기 ---
		cb.Update(context, dataB);
		cb.Bind(context, 0);
		context->DrawIndexed(model->GetIndexCount(), 0, 0);
		
		graphics.RenderEnd();
	}

	return 0;
}