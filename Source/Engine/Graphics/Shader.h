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
    void Create(ComPtr<ID3D11Device> device, const std::wstring& path, const std::string& vsName, const std::string& psName);

    void CreateVertexShader(ComPtr<ID3D11Device> device, const std::wstring& path, const std::string& vsName);
    void CreatePixelShader(ComPtr<ID3D11Device> device, const std::wstring& path, const std::string& psName);

    /**
     * @brief GPU 파이프라인에 이 쉐이더들을 장착합니다.
     */
    void Bind(ComPtr<ID3D11DeviceContext> context);

private:
    ComPtr<ID3D11VertexShader> _vertexShader;
    ComPtr<ID3D11PixelShader> _pixelShader;
    ComPtr<ID3D11InputLayout> _inputLayout; // InputLayout 은 Vertex의 정보들은 bytes 덩어리 이지만 어디까지가 위치이고 색상인지 말해주는 역할, CPU와 GPU 사이의 약속
};