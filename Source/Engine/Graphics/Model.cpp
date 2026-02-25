#include "Model.h"

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
        // 1. 앞면 (Normal: 0, 0, -1)
        { float3(-0.5f,  0.5f, -0.5f), float4(1, 0, 0, 1), float3(0, 0, -1) },
        { float3(0.5f,  0.5f, -0.5f), float4(0, 1, 0, 1), float3(0, 0, -1) },
        { float3(0.5f, -0.5f, -0.5f), float4(0, 0, 1, 1), float3(0, 0, -1) },
        { float3(-0.5f, -0.5f, -0.5f), float4(1, 1, 1, 1), float3(0, 0, -1) },

        // 2. 뒷면 (Normal: 0, 0, 1)
        { float3(-0.5f,  0.5f,  0.5f), float4(0, 1, 1, 1), float3(0, 0, 1) },
        { float3(0.5f,  0.5f,  0.5f), float4(1, 0, 1, 1), float3(0, 0, 1) },
        { float3(0.5f, -0.5f,  0.5f), float4(1, 1, 0, 1), float3(0, 0, 1) },
        { float3(-0.5f, -0.5f,  0.5f), float4(0, 0, 0, 1), float3(0, 0, 1) },

        // 3. 윗면 (Normal: 0, 1, 0)
        { float3(-0.5f,  0.5f,  0.5f), float4(1, 0, 0, 1), float3(0, 1, 0) },
        { float3(0.5f,  0.5f,  0.5f), float4(0, 1, 0, 1), float3(0, 1, 0) },
        { float3(0.5f,  0.5f, -0.5f), float4(0, 0, 1, 1), float3(0, 1, 0) },
        { float3(-0.5f,  0.5f, -0.5f), float4(1, 1, 1, 1), float3(0, 1, 0) },

        // 4. 아랫면 (Normal: 0, -1, 0)
        { float3(-0.5f, -0.5f, -0.5f), float4(0, 1, 1, 1), float3(0, -1, 0) },
        { float3(0.5f, -0.5f, -0.5f), float4(1, 0, 1, 1), float3(0, -1, 0) },
        { float3(0.5f, -0.5f,  0.5f), float4(1, 1, 0, 1), float3(0, -1, 0) },
        { float3(-0.5f, -0.5f,  0.5f), float4(0, 0, 0, 1), float3(0, -1, 0) },

        // 5. 왼쪽면 (Normal: -1, 0, 0)
        { float3(-0.5f,  0.5f,  0.5f), float4(1, 0, 0, 1), float3(-1, 0, 0) },
        { float3(-0.5f,  0.5f, -0.5f), float4(0, 1, 0, 1), float3(-1, 0, 0) },
        { float3(-0.5f, -0.5f, -0.5f), float4(0, 0, 1, 1), float3(-1, 0, 0) },
        { float3(-0.5f, -0.5f,  0.5f), float4(1, 1, 1, 1), float3(-1, 0, 0) },

        // 6. 오른쪽면 (Normal: 1, 0, 0)
        { float3(0.5f,  0.5f, -0.5f), float4(0, 1, 1, 1), float3(1, 0, 0) },
        { float3(0.5f,  0.5f,  0.5f), float4(1, 0, 1, 1), float3(1, 0, 0) },
        { float3(0.5f, -0.5f,  0.5f), float4(1, 1, 0, 1), float3(1, 0, 0) },
        { float3(0.5f, -0.5f, -0.5f), float4(0, 0, 0, 1), float3(1, 0, 0) }
    };

    // 2. 6개 면에 대한 인덱스 (삼각형 12개)
    // 시계 방향(CW)으로 정의해야 컬링에 안 잘립니다.
    std::vector<uint32_t> indices = {
    0, 1, 2, 0, 2, 3,       // 앞
    4, 5, 6, 4, 6, 7,       // 뒤
    8, 9, 10, 8, 10, 11,    // 위
    12, 13, 14, 12, 14, 15, // 아래
    16, 17, 18, 16, 18, 19, // 왼쪽
    20, 21, 22, 20, 22, 23  // 오른쪽
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