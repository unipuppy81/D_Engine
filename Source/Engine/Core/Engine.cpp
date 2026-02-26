#include "Engine/Core/Engine.h"

Engine::Engine()
	: m_window()      // Window 클래스의 생성자 호출
	, m_graphics()    // GraphicsDevice 클래스의 생성자 호출
{
	// 만약 GraphicsDevice(HWND) 처럼 인자가 필요한 생성자만 있다면
	// 여기 초기화 리스트에서 반드시 인자를 넣어줘야 합니다.
}

Engine::~Engine() {
	m_graphics.RenderEnd();
}

bool Engine::Initialize(HINSTANCE hInst, int width, int height) {
	// Window & Graphics Init
	if (!m_window.Create(L"D_Engine", width, height)) return false;
	m_graphics = GraphicsDevice(m_window.GetHWND());
	m_graphics.Initialize(width, height, true, m_window.GetHWND(), true);


	// Resources
	m_model = std::make_unique<Model>();
	if (!m_model->Initialize(m_graphics.GetDevice())) return false;
	m_colorShader = std::make_unique<ColorShader>();
	if (!m_colorShader->Initialize(m_graphics.GetDevice())) return false;

	m_rasterizerState.Create(m_graphics.GetDevice(), D3D11_FILL_SOLID, D3D11_CULL_NONE);

	// Camera & Data Set
	m_camera = std::make_unique<Camera>();
	m_camera->SetEye(0, 0, -10.0f);
	m_camera->SetLens(XM_PIDIV4, (float)width / (float)height, 0.1f, 1000.0f);

	m_transformA = std::make_unique<Transform>();
	m_transformB = std::make_unique<Transform>();
	m_transformA->SetPosition(-0.5f, 0.0f, 0.0f);
	m_transformB->SetPosition(0.5f, 0.0f, 0.0f);

	m_transformB->SetScale(5, 5, 5);
	m_transformA->SetParent(m_transformB.get());



	// Renderer Set
	m_rendererA = std::make_unique<MeshRenderer>();
	m_rendererA->SetModel(m_model.get());
	m_rendererA->SetShader(m_colorShader.get());
	m_rendererA->SetTransform(m_transformA.get());

	m_rendererB = std::make_unique<MeshRenderer>();
	m_rendererB->SetModel(m_model.get());
	m_rendererB->SetShader(m_colorShader.get());
	m_rendererB->SetTransform(m_transformB.get());



	// Timer Reset -> 모든 로딩이 끝난 지금을 0초로 설정
	m_timer.Reset();



	return true;
}

void Engine::Run() {
	// 루프 진입 직전에 시간을 한번 갱신해서 이전 프레임 시간을 현재로 맞춤
	m_timer.Tick();

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			m_timer.Tick(); // 여기서부터 진짜 프레임 간격 계산
			float dt = m_timer.GetDeltaTime();

			Update(dt);
			Render();
		}
	}
}

void Engine::Update(float dt) {
	m_camera->Update();

	
	static float rotation = 0.0f;
	rotation += 1.0f * dt; // 초당 1라디안 회전

	// Transform A: 오일러 각 사용
	m_transformA->SetRotation(rotation, rotation, 0.0f);

	// Transform B: 쿼터니언 사용 (qA만 남기고 덮어쓰기 제거)
	Quaternion qB = Quaternion::FromEuler(0.0f, -rotation, 0.0f);
	m_transformB->SetRotation(qB);
}

void Engine::Render() {
	m_graphics.RenderBegin(0.1f, 0.1f, 0.1f, 1.0f);
	auto context = m_graphics.GetContext();

	m_rasterizerState.Bind(context);

	Matrix view = m_camera->GetViewMatrix();
	Matrix proj = m_camera->GetProjectionMatrix();

	m_rendererA->Render(context, view, proj);
	m_rendererB->Render(context, view, proj);

	m_graphics.RenderEnd();
}

void Engine::Finalize() {
	// 1. Graphics 자원 정리 (필요 시)
	// m_graphics.Shutdown(); // 만약 내부에서 Release를 명시적으로 해야 한다면 호출

	// 2. 스마트 포인터들은 Engine 객체가 파괴될 때 자동으로 소멸되지만,
	// 명시적으로 순서를 정하고 싶다면 여기서 .reset()을 호출해도 됩니다.
	m_rendererA.reset();
	m_rendererB.reset();
	m_model.reset();
	m_colorShader.reset();
}