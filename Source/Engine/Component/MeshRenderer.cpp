#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() {}
MeshRenderer::~MeshRenderer() {}

void MeshRenderer::Render(ComPtr<ID3D11DeviceContext> context, const Matrix& view, const Matrix& proj) {
    if (!_model || !_shader || !_transform) return;

    // TransformData 준비 및 전송 (b0)
    TransformData tData;
    Matrix world = _transform->GetWorldMatrix();

    tData.matWorld = world.Transpose().ToSimd();
    tData.matView = view.ToSimd();
    tData.matProjection = proj.ToSimd();

    // 메쉬(정점/인덱스) 버퍼 세팅
    _model->Render(context);        

    // LigtData 준비
    LightData lData;
    // 1. 빛의 색상 (RGBA: 흰색)
    lData.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

    // 2. 빛이 나가는 방향 (Vector3)
    // (0, -1, 0)이면 하늘에서 땅으로 수직으로 떨어지는 빛입니다.
    // 약간 대각선으로 줘야 물체의 입체감이 잘 살아납니다.
    lData.direction = XMFLOAT3(0.5f, -1.0f, 0.5f);

    // 3. 패딩 (구조체에서 만든 float padding)
    lData.padding = 0.0f;

    // 상수 버퍼 업데이트 및 셰이더 실행
    _shader->Render(context, tData, lData);      

    // 그리기
    context->DrawIndexed(_model->GetIndexCount(), 0, 0);
}