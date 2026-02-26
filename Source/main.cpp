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