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
#include "Engine/Core/Camera.h"
#include "Engine/Core/MeshRenderer.h"

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
	RasterizerState rs;
	rs.Create(graphics.GetDevice(), D3D11_FILL_SOLID, D3D11_CULL_NONE);


	// 실시간 데이터
	Transform* transformA = new Transform();
	Transform* transformB = new Transform();
	transformA->SetPosition(-0.5f, 0.0f, 0.0f);
	transformB->SetPosition(0.5f, 0.0f, 0.0f);

	Camera* camera = new Camera();
	camera->SetEye(0, 0, -10.0f);		// 카메라 위치


	MeshRenderer* rendererA = new MeshRenderer();
	rendererA->SetModel(model);
	rendererA->SetShader(cs);
	rendererA->SetTransform(transformA);

	MeshRenderer* rendererB = new MeshRenderer();
	rendererB->SetModel(model);
	rendererB->SetShader(cs);
	rendererB->SetTransform(transformB);

	// 게임 루프
	while (window.Run()) {
		graphics.RenderBegin(0.1f, 0.1f, 0.1f, 1.0f);
		auto context = graphics.GetContext();
		
		// 공통 상태 설정 (루프당 한 번)
		rs.Bind(context);

		camera->Update();

		// --- 데이터 업데이트 (행렬 수동 계산 X) ---
		static float rotation = 0.0f;
		rotation += 0.01f;
		transformA->SetRotation(rotation, rotation, 0.0f);
		transformB->SetRotation(0.0f, -rotation, 0.0f);
		
		// 카메라 행렬 한 번만 준비 (V, P)
		XMMATRIX view = camera->GetViewMatrix();
		XMMATRIX proj = camera->GetProjectionMatrix();

		// 렌더링 (그리는 순서, 행렬 조립, Draw호출을 Renderer가 알아서 함)
		rendererA->Render(context, view, proj);
		rendererB->Render(context, view, proj);

		graphics.RenderEnd();
	}

	return 0;
}