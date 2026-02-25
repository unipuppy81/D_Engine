#include "Shader.h"
#include <d3dcompiler.h>

void Shader::Create(ComPtr<ID3D11Device> device, const std::wstring& path, const std::string& vsName, const std::string& psName) {
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
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    device->CreateInputLayout(layoutDesc, 3, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &_inputLayout);
}

void Shader::CreateVertexShader(ComPtr<ID3D11Device> device, const std::wstring& path, const std::string& vsName) {
    ComPtr<ID3DBlob> vsBlob;
    ComPtr<ID3DBlob> errorBlob;

    HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, vsName.c_str(), "vs_5_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr)) {
        if (errorBlob) MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "VS Compile Error", MB_OK);
        return;
    }
    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &_vertexShader);

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    device->CreateInputLayout(layoutDesc, 3, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &_inputLayout);
}

void Shader::CreatePixelShader(ComPtr<ID3D11Device> device, const std::wstring& path, const std::string& psName) {
    ComPtr<ID3DBlob> psBlob;
    ComPtr<ID3DBlob> errorBlob;

    HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, psName.c_str(), "ps_5_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr)) {
        if (errorBlob) MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "PS Compile Error", MB_OK);
        return;
    }
    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &_pixelShader);
}

void Shader::Bind(ComPtr<ID3D11DeviceContext> context) {
    if(_inputLayout) context->IASetInputLayout(_inputLayout.Get());
    if(_vertexShader) context->VSSetShader(_vertexShader.Get(), nullptr, 0);
    if(_pixelShader) context->PSSetShader(_pixelShader.Get(), nullptr, 0);
}