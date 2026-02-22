#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <vector>

using Microsoft::WRL::ComPtr;

// 버텍스 쉐이더와 픽셀 쉐이더의 컴파일 및 장착을 담당하는 클래스
class Shader {
public:
	Shader() = default;
	~Shader() = default;

    /**
     * @brief HLSL 파일을 읽어 쉐이더를 생성하고 입력 레이아웃까지 준비합니다.
     * @param device 장치 객체
     * @param path 쉐이더 파일 경로
     * @param vsName 버텍스 쉐이더 진입점 (예: "VS")
     * @param psName 픽셀 쉐이더 진입점 (예: "PS")
     */
    void Create(ComPtr<ID3D11Device> device, const std::wstring& path, const std::string& vsName, const std::string& psName) {
        ComPtr<ID3DBlob> vsBlob;
        ComPtr<ID3DBlob> psBlob;
        ComPtr<ID3DBlob> errorBlob;

        // 1. 버텍스 쉐이더 컴파일
        HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, nullptr, vsName.c_str(), "vs_5_0", 0, 0, &vsBlob, &errorBlob);
        if (FAILED(hr)) {
            if (errorBlob) MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "VS Compile Error", MB_OK);
            return;
        }
        device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &_vertexShader);

        // 2. 픽셀 쉐이더 컴파일
        hr = D3DCompileFromFile(path.c_str(), nullptr, nullptr, psName.c_str(), "ps_5_0", 0, 0, &psBlob, &errorBlob);
        if (FAILED(hr)) {
            if (errorBlob) MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "PS Compile Error", MB_OK);
            return;
        }
        device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &_pixelShader);

        // 3. 입력 레이아웃(Input Layout) 생성 
        // (현재는 위치+색상 구조로 고정하지만, 나중에 13번 항목에서 확장 가능하게 바꿀 수 있습니다)
        D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
            // [항목 이름], [인덱스], [데이터 타입], [슬롯], [시작 지점(Offset)], [분류], [인스턴스데이터]
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        device->CreateInputLayout(layoutDesc, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &_inputLayout);
    }

    /**
     * @brief GPU 파이프라인에 이 쉐이더들을 장착합니다.
     */
    void Bind(ComPtr<ID3D11DeviceContext> context) {
        context->IASetInputLayout(_inputLayout.Get());
        context->VSSetShader(_vertexShader.Get(), nullptr, 0);
        context->PSSetShader(_pixelShader.Get(), nullptr, 0);
    }

private:
    ComPtr<ID3D11VertexShader> _vertexShader;
    ComPtr<ID3D11PixelShader> _pixelShader;
    ComPtr<ID3D11InputLayout> _inputLayout; // InputLayout 은 Vertex의 정보들은 bytes 덩어리 이지만 어디까지가 위치이고 색상인지 말해주는 역할, CPU와 GPU 사이의 약속
};