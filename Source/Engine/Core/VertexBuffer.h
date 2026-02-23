#pragma once
#include <vector>
#include <wrl/client.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;


 // 정점 데이터를 GPU 메모리에 저장하고 관리하는 클래스
class VertexBuffer {
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;


    /**
     // CPU의 정점 데이터를 바탕으로 GPU 정점 버퍼를 생성합니다.
     * @tparam T 정점 구조체 타입 (예: Vertex)
     * @param device 장치 객체 (버퍼 생성용)
     * @param vertices 정점 데이터 리스트 (std::vector)
     */
	template<typename T>
	bool Create(ComPtr<ID3D11Device> device, const std::vector<T>& vertices) {
        // 1. 정점 하나당 크기(Stride)와 전체 개수(Count) 저장
        _stride = sizeof(T);
        _count = (uint32_t)vertices.size();

        // 2. 버퍼의 설정값(Description) 정의
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;         // GPU가 읽고 쓸 수 있는 기본 설정
        bd.ByteWidth = _stride * _count;        // 버퍼의 전체 용량 (바이트 단위)
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // "이 버퍼는 정점 전용이다"라고 명시
        bd.CPUAccessFlags = 0;                  // 생성 후 CPU에서 접근할 필요 없음

        // 3. 복사할 원본 데이터 지정
        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vertices.data();     // 실제 정점 데이터가 있는 메모리 주소

        // 4. GPU 메모리에 실제 버퍼 생성
        HRESULT hr = device->CreateBuffer(&bd, &initData, _buffer.GetAddressOf());

        if (FAILED(hr)) {
            return false;
        }

        return true;
	}

    // 이 버퍼를 GPU 파이프라인(Input Assembler)에 장착합니다.
    void Bind(ComPtr<ID3D11DeviceContext> context) {
        uint32_t offset = 0;
        context->IASetVertexBuffers(0, 1, _buffer.GetAddressOf(), &_stride, &offset);
    }

private:
    ComPtr<ID3D11Buffer> _buffer;   // GPU 버퍼 객체
    uint32_t _stride = 0;           // 정점 1개당 크기
    uint32_t _count = 0;            // 전체 정점 개수
};