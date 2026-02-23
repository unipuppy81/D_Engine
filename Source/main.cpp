#include "Engine/Core/Window.h"
#include "Engine/Core/GraphicsDevice.h"
#include "Engine/Core/VertexBuffer.h"
#include "Engine/Core/IndexBuffer.h"
#include "Engine/Core/Shader.h"
#include "Engine/Core/ConstantBuffer.h"
#include "Engine/Core/RasterizerState.h"
#include "Engine/Core/Model.h"
#include "Engine/Core/ColorShader.h"

#include <d3dcompiler.h>
#include <memory>
#include <vector>

int main() {
	// 초기화
	Window window;
	if (!window.Create(L"D_Engine", 1280, 720)) return -1;
	GraphicsDevice graphics(window.GetHWND());
	graphics.Init(1280, 720, true, window.GetHWND(), true);


	// 도구 준비
	Model* model = new Model();
	if (!model->Initialize(graphics.GetDevice())) return -1;

	ColorShader* cs = new ColorShader();
	if (!cs->Initialize(graphics.GetDevice())) return -1;

	// 일반적인 설정: 면을 채우고(SOLID), 뒷면은 그리지 마라(BACK)
	// D3D11_FILL_WIREFRAME(디버그용), D3D11_FILL_SOLID (기본값)
	// D3D11_CULL_BACK vs D3D11_CULL_NONE (컬링)
	RasterizerState rs;
	rs.Create(graphics.GetDevice(), D3D11_FILL_SOLID, D3D11_CULL_NONE);

	// 실시간 데이터
	TransformData dataA, dataB;

	// 게임 루프
	while (window.Run()) {
		graphics.RenderBegin(0.1f, 0.1f, 0.1f, 1.0f);
		auto context = graphics.GetContext();
		
		// 공통 상태 설정 (루프당 한 번)
		rs.Bind(context);

		// 무엇을 그릴지 먼저 세팅 (내부에서 vb, ib, topology 셋팅)
		model->Render(context);

		static float rotation = 0.0f;
		rotation += 0.01f; // 매 프레임 조금씩 회전

		// 1. World 행렬: 큐브를 회전시키고 위치를 잡음
		// X, Y축으로 동시에 회전시키고, X축으로 -0.2f만큼 이동
		XMMATRIX matWorldA = XMMatrixRotationY(rotation) * XMMatrixRotationX(rotation) * XMMatrixTranslation(-0.2f, 0.0f, 0.0f);
		XMMATRIX matWorldB = XMMatrixRotationY(-rotation) * XMMatrixTranslation(0.2f, 0.0f, 0.5f);

		// 2. View 행렬: 카메라 위치 (0, 0, -5)에서 (0, 0, 0)을 바라봄
		XMMATRIX matView = XMMatrixLookAtLH(
			XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		);


		// 3. Projection 행렬: 원근감 (화각 90도, 화면비율 16:9 기준)
		XMMATRIX matProj = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1280.0f / 720.0f, 0.1f, 1000.0f);


		// 4. 데이터 조립 및 전치(Transpose)
		// HLSL은 열 우선(Column-Major)이므로 Transpose가 필수입니다!
		dataA.matWorld = XMMatrixTranspose(matWorldA);
		dataA.matView = XMMatrixTranspose(matView);
		dataA.matProjection = XMMatrixTranspose(matProj);

		dataB.matWorld = XMMatrixTranspose(matWorldB);
		dataB.matView = XMMatrixTranspose(matView);
		dataB.matProjection = XMMatrixTranspose(matProj);

		// --- 첫 번째 사각형 (A) 그리기 ---
		// 어떻게 그릴지(Shader) + 데이터(dataA) 세팅 후 그리기
		cs->Render(context, dataA);
		context->DrawIndexed(model->GetIndexCount(), 0, 0);

		// --- 두 번째 사각형 (B) 그리기 ---
		// 데이터만 바꿔서(dataB) 다시 세팅 후 그리기
		cs->Render(context, dataB);
		context->DrawIndexed(model->GetIndexCount(), 0, 0);
		
		graphics.RenderEnd();
	}

	return 0;
}