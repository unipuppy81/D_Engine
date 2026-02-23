#include "Model.h"
#include "Vertex.h"

bool Model::Initialize(ComPtr<ID3D11Device> device) {
    // 1. 실제 버퍼(Vertex/Index) 생성 로직 호출
    if (!InitializeBuffers(device)) {
        return false;
    }

    return true;
}

void Model::Shutdown() {
    // ComPtr 멤버들은 소멸자에서 자동 해제되지만, 
    // 명시적으로 관리하고 싶다면 여기서 Reset() 만들어 호출
}

void Model::Render(ComPtr<ID3D11DeviceContext> context) {
    // 2. GPU 파이프라인(Input Assembler)에 이 모델의 버퍼들을 장착
    _vertexBuffer.Bind(context);
    _indexBuffer.Bind(context);

    // 3. 토폴로지 설정 (삼각형 리스트로 그리겠다)
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::InitializeBuffers(ComPtr<ID3D11Device> device) {
    // 나중에 이 부분은 파일 로드(FBX, OBJ 등)로 대체될 영역입니다.
    // 1. 8개의 꼭짓점 정의 (앞면 4개, 뒷면 4개)
    std::vector<Vertex> vertices = {
        // 앞면 (Z = -0.5f)
        { float3(-0.5f,  0.5f, -0.5f), float4(1.0f, 0.0f, 0.0f, 1.0f) }, // 0: 좌상
        { float3(0.5f,  0.5f, -0.5f), float4(0.0f, 1.0f, 0.0f, 1.0f) }, // 1: 우상
        { float3(0.5f, -0.5f, -0.5f), float4(0.0f, 0.0f, 1.0f, 1.0f) }, // 2: 우하
        { float3(-0.5f, -0.5f, -0.5f), float4(1.0f, 1.0f, 1.0f, 1.0f) }, // 3: 좌하

        // 뒷면 (Z = 0.5f)
        { float3(-0.5f,  0.5f,  0.5f), float4(0.0f, 1.0f, 1.0f, 1.0f) }, // 4: 좌상
        { float3(0.5f,  0.5f,  0.5f), float4(1.0f, 0.0f, 1.0f, 1.0f) }, // 5: 우상
        { float3(0.5f, -0.5f,  0.5f), float4(1.0f, 1.0f, 0.0f, 1.0f) }, // 6: 우하
        { float3(-0.5f, -0.5f,  0.5f), float4(0.0f, 0.0f, 0.0f, 1.0f) }  // 7: 좌하
    };

    // 2. 6개 면에 대한 인덱스 (삼각형 12개)
    // 시계 방향(CW)으로 정의해야 컬링에 안 잘립니다.
    std::vector<uint32_t> indices = {
        // 앞면
        0, 1, 2,  0, 2, 3,
        // 뒷면
        5, 4, 7,  5, 7, 6,
        // 윗면
        4, 5, 1,  4, 1, 0,
        // 아랫면
        3, 2, 6,  3, 6, 7,
        // 왼쪽면
        4, 0, 3,  4, 3, 7,
        // 오른쪽면
        1, 5, 6,  1, 6, 2
    };
    _indexCount = static_cast<int>(indices.size());

    // 실제 GPU 버퍼 객체 생성
    if (!_vertexBuffer.Create(device, vertices)) {
        return false;
    }

    if (!_indexBuffer.Create(device, indices)) {
        return false;
    }

    return true;
}