#pragma once
#include <wrl/client.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

// CPU에서 GPU 셰이더로 데이터를 전송하기 위한 상수 버퍼 클래스
// 한 번의 DrawCall 이 끝날때까지 값이 변하지 않고(Constant) 유지되는 버퍼
// C++ 에서 템플릿은 설계도라서 타입이 들어오기 전까지 컴파일러가 실제 코드 만들 수 없음 -> .cpp X
template<typename T>
class ConstantBuffer {
public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

	// 상수 버퍼 생성
    bool Create(ComPtr<ID3D11Device> device) {
        // 상수 버퍼는 크기가 반드시 16바이트의 배수여야 합니다. (DirectX 11 규칙)
        // (size + 15) & ~15 는 16의 배수로 올림하는 비트 연산
        uint32_t size = (sizeof(T) + 15) & ~15;

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DYNAMIC;             // CPU가 매 프레임 수정 가능
        bd.ByteWidth = size;                        // 보정된 사이즈 입력
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // 상수 버퍼 선언
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU에서 쓰기 권한 허용

        HRESULT hr = device->CreateBuffer(&bd, nullptr, _buffer.ReleaseAndGetAddressOf());

        if (FAILED(hr)) {
            OutputDebugString(L"상수 버퍼 Map 실패\n");
            return false;
        }

        return true;
    }

    // CPU의 데이터를 GPU 버퍼로 복사(업데이트)
    void Update(ComPtr<ID3D11DeviceContext> context, const T& data) {
        if (!_buffer) return;

        D3D11_MAPPED_SUBRESOURCE ms;
        // Map 성공 여부를 체크
        HRESULT hr = context->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
        if (SUCCEEDED(hr)) {
            memcpy(ms.pData, &data, sizeof(T));
            context->Unmap(_buffer.Get(), 0);
        }
    }

    // 쉐이더의 특정 슬롯에 이 버퍼를 장착
    void BindVS(ComPtr<ID3D11DeviceContext> context, uint32_t slot) {
        // 버텍스 셰이더(VS)의 상수 버퍼 슬롯에 장착
        context->VSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
    }

    void BindPS(ComPtr<ID3D11DeviceContext> context, uint32_t slot) {
        // 픽셀 셰이더(PS)의 상수 버퍼 슬롯에 장착
        context->PSSetConstantBuffers(slot, 1, _buffer.GetAddressOf());
    }


private:
    ComPtr<ID3D11Buffer> _buffer;
};