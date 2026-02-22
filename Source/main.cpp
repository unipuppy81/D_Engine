#include "Engine/Core/Window.h"
#include "Engine/Core/GraphicsDevice.h"
#include <memory>

int main() {
	Window window;
	// 윈도우 창 생성
	if (!window.Create(L"D_Engine", 1280, 720)) return -1;

	GraphicsDevice graphics(window.GetHWND());
	graphics.Init();

	// 게임 루프
	while (window.Run()) {
		graphics.RenderBegin();
		// 여기 Renderer->Render() 추가 예정
		graphics.RenderEnd();
	}

	return 0;
}