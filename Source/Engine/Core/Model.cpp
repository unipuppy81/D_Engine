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
    // 4. 정점 데이터 준비 (사각형)
    // 나중에 이 부분은 파일 로드(FBX, OBJ 등)로 대체될 영역입니다.
    std::vector<Vertex> vertices = {
        { float3(-0.5f,  0.5f, 0.0f), float4(1.0f, 0.0f, 0.0f, 1.0f) },  // 0: 왼쪽 위
        { float3(0.5f,  0.5f, 0.0f), float4(0.0f, 1.0f, 0.0f, 1.0f) },   // 1: 오른쪽 위
        { float3(0.5f, -0.5f, 0.0f), float4(0.0f, 0.0f, 1.0f, 1.0f) },   // 2: 오른쪽 아래
        { float3(-0.5f, -0.5f, 0.0f), float4(1.0f, 1.0f, 1.0f, 1.0f) }   // 3: 왼쪽 아래
    };

    // 5. 인덱스 데이터 준비 (시계 방향)
    std::vector<uint32_t> indices = {
        0, 1, 2,
        0, 2, 3
    };
    _indexCount = static_cast<int>(indices.size());

    // 6. 실제 GPU 버퍼 객체 생성 (기존에 구현하신 클래스 활용)
    if (!_vertexBuffer.Create(device, vertices)) {
        return false;
    }

    if (!_indexBuffer.Create(device, indices)) {
        return false;
    }

    return true;
}