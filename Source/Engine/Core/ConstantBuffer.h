#pragma once
#include <wrl/client.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

// CPU에서 GPU 셰이더로 데이터를 전송하기 위한 상수 버퍼 클래스
// 한 번의 DrawCall 이 끝날때까지 값이 변하지 않고(Constant) 유지되는 버퍼
template<typename T>
class ConstantBuffer {
public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

	// 상수 버퍼 생성
    void Create(ComPtr<ID3D11Device> device) {
        // 상수 버퍼는 크기가 반드시 16바이트의 배수여야 합니다. (DirectX 11 규칙)
        uint32_t size = sizeof(T);

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DYNAMIC;         // CPU가 매 프레임 수정할 수 있도록 설정
        bd.ByteWidth = size;
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // "이것은 상수 버퍼다"
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU에서 쓰기 권한 허용

        device->CreateBuffer(&bd, nullptr, _buffer.GetAddressOf());
    }

    // CPU의 데이터를 GPU 버퍼로 복사(업데이트)
    void Update(ComPtr<ID3D11DeviceContext> context, const T& data) {
        D3D11_MAPPED_SUBRESOURCE ms;
        // GPU가 사용 중이지 않을 때 버퍼를 잠그고(Map) 데이터를 씁니다.
        context->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
        memcpy(ms.pData, &data, sizeof(T));
        context->Unmap(_buffer.Get(), 0);
    }

    // 쉐이더의 특정 슬롯에 이 버퍼를 장착
    void Bind(ComPtr<ID3D11DeviceContext> context, uint32_t slot) {
        // 버텍스 셰이더(VS)의 상수 버퍼 슬롯에 장착
        context->VSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
    }


private:
    ComPtr<ID3D11Buffer> _buffer;
};