//#include "Engine/Core/Window.h"
//#include "Engine/Core/Timer.h"
//#include "Engine/Graphics/GraphicsDevice.h"
//#include "Engine/Graphics/VertexBuffer.h"
//#include "Engine/Graphics/IndexBuffer.h"
//#include "Engine/Graphics/Shader.h"
//#include "Engine/Graphics/ConstantBuffer.h"
//#include "Engine/Graphics/RasterizerState.h"
//#include "Engine/Graphics/Model.h"
//#include "Engine/Graphics/ColorShader.h"
//#include "Engine/Component/Transform.h"
//#include "Engine/Component/Camera.h"
//#include "Engine/Component/MeshRenderer.h"
#include "Engine/Core/Engine.h"

#include <d3dcompiler.h>
#include <memory>
#include <vector>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	// Engine Start
	if (!Engine::Get().Initialize(hInstance, 1280, 720)) return -1;

	// 루프 실행
	Engine::Get().Run();

	// 자원 해제
	Engine::Get().Finalize();

	return 0;
}
/*
int main() {
	// 초기화
	Window window;
	if (!window.Create(L"D_Engine", 1280, 720)) return -1;
	GraphicsDevice graphics(window.GetHWND());
	graphics.Initialize(1280, 720, true, window.GetHWND(), true);



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
*/