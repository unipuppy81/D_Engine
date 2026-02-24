#include "Engine/Core/Window.h"
#include "Engine/Core/GraphicsDevice.h"
#include "Engine/Core/VertexBuffer.h"
#include "Engine/Core/IndexBuffer.h"
#include "Engine/Core/Shader.h"
#include "Engine/Core/ConstantBuffer.h"
#include "Engine/Core/RasterizerState.h"
#include "Engine/Core/Model.h"
#include "Engine/Core/ColorShader.h"
#include "Engine/Core/Transform.h"

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
	Transform* transformA = new Transform();
	Transform* transformB = new Transform();
	transformA->SetPosition(-0.5f, 0.0f, 0.0f);
	transformB->SetPosition(0.5f, 0.0f, 0.0f);


	// 게임 루프
	while (window.Run()) {
		graphics.RenderBegin(0.1f, 0.1f, 0.1f, 1.0f);
		auto context = graphics.GetContext();
		
		// 공통 상태 설정 (루프당 한 번)
		rs.Bind(context);

		// 무엇을 그릴지 먼저 세팅 (내부에서 vb, ib, topology 셋팅)
		model->Render(context);


		static float rotation = 0.0f;
		rotation += 0.01f;
	
		// --- 데이터 업데이트 (행렬 수동 계산 X) ---
		transformA->SetRotation(rotation, rotation, 0.0f);
		transformB->SetRotation(0.0f, -rotation, 0.0f);
		
		// --- 데이터 조립 ---
		// World 행렬: Transform 클래스가 SRT 곱하기 + Transpose까지 해서 줍니다.
		dataA.matWorld = transformA->GetWorldMatrix();
		dataB.matWorld = transformB->GetWorldMatrix();

		// --- 뷰/투영 행렬 (카메라 캡슐화 전이라 일단 수동 계산) ---
		XMMATRIX matView = XMMatrixLookAtLH(
			XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		);
		XMMATRIX matProj = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1280.0f / 720.0f, 0.1f, 1000.0f);


		// 데이터 조립 및 전치(Transpose)
		// HLSL은 열 우선(Column-Major)이므로 Transpose가 필수입니다!
		dataA.matView = XMMatrixTranspose(matView);
		dataA.matProjection = XMMatrixTranspose(matProj);

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