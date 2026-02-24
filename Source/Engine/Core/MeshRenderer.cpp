#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() {}
MeshRenderer::~MeshRenderer() {}

void MeshRenderer::Render(ComPtr<ID3D11DeviceContext> context, const XMMATRIX& view, const XMMATRIX& proj) {
    if (!_model || !_shader || !_transform) return;

    // TransformData 준비 및 전송 (b0)
    TransformData tData;
    tData.matWorld = _transform->GetWorldMatrix();
    tData.matView = view;        // 외부(카메라)에서 받은 값 사용
    tData.matProjection = proj;  // 외부(카메라)에서 받은 값 사용
    _model->Render(context);         // 메쉬(정점/인덱스) 버퍼 세팅

    LightData lData;
    // 1. 빛의 색상 (RGBA: 흰색)
    lData.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

    // 2. 빛이 나가는 방향 (Vector3)
    // (0, -1, 0)이면 하늘에서 땅으로 수직으로 떨어지는 빛입니다.
    // 약간 대각선으로 줘야 물체의 입체감이 잘 살아납니다.
    lData.direction = XMFLOAT3(0.5f, -1.0f, 0.5f);

    // 3. 패딩 (구조체에서 만든 float padding)
    lData.padding = 0.0f;


    _shader->Render(context, tData, lData);       // 셰이더 상수 버퍼 업데이트 및 바인딩, 이 안에 VsSetConstantBuffers(0, ...) 들어있음




    // 그리기
    context->DrawIndexed(_model->GetIndexCount(), 0, 0);
}