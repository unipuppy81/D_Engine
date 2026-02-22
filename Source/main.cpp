#include "Engine/Core/Window.h"
#include "Engine/Core/GraphicsDevice.h"
#include <d3dcompiler.h>
#include <memory>

// 정점 구조체
struct Vertex {
	float x, y, z;
	float r, g, b, a;
};

int main() {
	Window window;
	// 윈도우 창 생성
	if (!window.Create(L"D_Engine", 1280, 720)) return -1;

	// GrphicsDevice 초기화
	GraphicsDevice graphics(window.GetHWND());
	graphics.Init();

	// 정점 데이터
	Vertex vertices[] = {
		{ -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // 0: 왼쪽 위
		{  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // 1: 오른쪽 위
		{  0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }, // 2: 오른쪽 아래
		{ -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f }  // 3: 왼쪽 아래
	};

	// 인덱스 데이터
	uint32_t indices[] = {
		0, 1, 2, 
		0, 2, 3 
	};


	// 1. 쉐이더 컴파일 (Default.hlsl 파일 읽어서 GPU가 이해하는 기계어로 변환)
	ComPtr<ID3DBlob> vsBlob;	// 컴파일된 버텍스 쉐이더 담을 바구니
	ComPtr<ID3DBlob> psBlob;	// 컴파일된 픽셀 쉐이더 담을 바구니
	ComPtr<ID3DBlob> errorBlob;	// 에러 담는 바구니

	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;

	// 버텍스 쉐이더 컴파일 (VS 함수 찾기)
	D3DCompileFromFile(L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/Default.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &vsBlob, nullptr);
	graphics.GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);


	// 픽셀 셰이더 컴파일 (PS 함수 찾기)
	D3DCompileFromFile(L"C:/Users/unipu/Documents/GitHub/D_Engine/Resources/Shaders/Default.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &psBlob, nullptr);
	graphics.GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);



	// 2. 입력 레이아웃 설정(GPU에 우리 데이터는 [위치3개+색상4개] 구조라고 알림
	ComPtr<ID3D11InputLayout> inputLayout;
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	graphics.GetDevice()->CreateInputLayout(layoutDesc, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);


	// 3. 정점 버퍼(Vertex Buffer) 생성 (삼각형 데이터를 GPU 메모리에 복사)
	ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices); // vertices 배열의 전체 크기
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices; // CPU에 있는 데이터 주소
	graphics.GetDevice()->CreateBuffer(&bd, &initData, &vertexBuffer);
	

	// 3-1 인덱스 버퍼 생성
	ComPtr<ID3D11Buffer> indexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(indices);			// 인덱스 배열 전체 크기
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;	// 반드시 인덱스 버퍼로 설정

	D3D11_SUBRESOURCE_DATA iInitData = {};
	iInitData.pSysMem = indices;           // 인덱스 데이터 주소
	graphics.GetDevice()->CreateBuffer(&ibd, &iInitData, &indexBuffer);


	// 게임 루프
	while (window.Run()) {
		graphics.RenderBegin();


		auto context = graphics.GetContext();
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		context->IASetInputLayout(inputLayout.Get());                    // 뼈대 설정
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 삼각형으로 그리겠다
		
		
		// 1. Vertex Buffer Connect
		context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset); // 정점 전달
		
		// 2. Index Buffer Connect
		// DXGI_FORMAT_R32_UINT -> uint32_t 사용한다는 의미
		context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		
		context->VSSetShader(vertexShader.Get(), nullptr, 0);            // 버텍스 셰이더 장착
		context->PSSetShader(pixelShader.Get(), nullptr, 0);            // 픽셀 셰이더 장착


		//context->Draw(3, 0);		// Non Index Buffer
		context->DrawIndexed(6, 0, 0);	// Use Index Buffer
		
		graphics.RenderEnd();
	}

	return 0;
}