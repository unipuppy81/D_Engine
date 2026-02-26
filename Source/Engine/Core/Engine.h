#pragma once
#include "Engine/Core/Window.h"
#include "Engine/Core/Timer.h"
#include "Engine/Component/MeshRenderer.h"
#include "Engine/Component/Transform.h"
#include "Engine/Component/Camera.h"
#include "Engine/Graphics/ColorShader.h"
#include "Engine/Graphics/RasterizerState.h"
#include "Engine/Graphics/GraphicsDevice.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Math/Matrix.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Math/Vector.h"


#include <memory>

using namespace std;

class Engine {
public:
	static Engine& Get() {
		static Engine instance;
		return instance;
	}

	// 흐름 제어
	bool Initialize(HINSTANCE hInst, int width, int height);
	void Run();			// 메인 루프
	void Finalize();	// 해제


private:
	Engine();
	~Engine();

	void Update(float dt);
	void Render();


private:
	// Core
	Window m_window;
	GraphicsDevice m_graphics;
	Timer m_timer;

	bool m_isTerminated = false;


	// Resource & Renderer (smart pointer)
	std::unique_ptr<Model>          m_model;
	std::unique_ptr<ColorShader>    m_colorShader;
	RasterizerState                 m_rasterizerState;

	std::unique_ptr<Camera>         m_camera;

	// 객체 A, B
	std::unique_ptr<Transform>      m_transformA;
	std::unique_ptr<Transform>      m_transformB;
	std::unique_ptr<MeshRenderer>   m_rendererA;
	std::unique_ptr<MeshRenderer>   m_rendererB;
};