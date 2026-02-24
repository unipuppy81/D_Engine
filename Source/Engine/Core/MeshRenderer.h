#pragma once
#include "Model.h"
#include "ColorShader.h"
#include "Transform.h"

class MeshRenderer {
public:
	MeshRenderer();
	~MeshRenderer();

	// 필요 부품 장책
	void SetModel(Model* model) { _model = model; }
	void SetShader(ColorShader* shader) { _shader = shader; }
	void SetTransform(Transform* transform) { _transform = transform; }

	// 그리기 (행렬 조립 + 바인딩 + 드로우)
	void Render(ComPtr<ID3D11DeviceContext> context, const XMMATRIX& view, const XMMATRIX& proj);

private:
	Model* _model = nullptr;
	ColorShader* _shader = nullptr;
	Transform* _transform = nullptr;
};