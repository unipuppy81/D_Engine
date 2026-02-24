#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() {}
MeshRenderer::~MeshRenderer() {}

void MeshRenderer::Render(ComPtr<ID3D11DeviceContext> context, const XMMATRIX& view, const XMMATRIX& proj) {
    if (!_model || !_shader || !_transform) return;

    // 1. 이 물체만의 전용 데이터 조립 (TransformData)
    TransformData data;
    data.matWorld = _transform->GetWorldMatrix();
    data.matView = view;        // 외부(카메라)에서 받은 값 사용
    data.matProjection = proj;  // 외부(카메라)에서 받은 값 사용

    // 2. 메쉬(정점/인덱스) 버퍼 세팅
    _model->Render(context);

    // 3. 셰이더 상수 버퍼 업데이트 및 바인딩
    _shader->Render(context, data);

    // 4. 최종 그리기
    context->DrawIndexed(_model->GetIndexCount(), 0, 0);
}