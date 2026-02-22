#pragma once
#include <vector>
#include <wrl/client.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

// 정점들을 어떤 순서로 그릴지 결정하는 '인덱스 번호표' 버퍼 클래스
class IndexBuffer {
public:
    IndexBuffer() = default;
    ~IndexBuffer() = default;

    //정점 인덱스 데이터를 바탕으로 GPU 인덱스 버퍼를 생성합니다.
    //인덱스 데이터 (uint32_t 리스트)
    void Create(ComPtr<ID3D11Device> device, const std::vector<uint32_t>& indices) {
        _count = (uint32_t)indices.size();

        // 1. 버퍼의 설정값 정의
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(uint32_t) * _count; // 인덱스(4바이트) * 개수
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;   // "이 버퍼는 인덱스 전용이다"라고 명시

        // 2. 복사할 데이터 주소 연결
        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = indices.data();

        // 3. GPU 버퍼 생성
        device->CreateBuffer(&bd, &initData, _buffer.GetAddressOf());
    }

    // 이 인덱스 버퍼를 GPU 파이프라인에 장착합니다.
    void Bind(ComPtr<ID3D11DeviceContext> context) {
        // 인덱스 버퍼는 별도의 전용 함수를 사용함 (R32_UINT는 uint32_t 타입임을 의미)
        context->IASetIndexBuffer(_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }

    uint32_t GetCount() const { return _count; }

private:
    ComPtr<ID3D11Buffer> _buffer;
    uint32_t _count = 0;
};