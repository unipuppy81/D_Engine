#pragma once
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Vertex.h"

class ColorShader {
public:
    bool Initialize(ComPtr<ID3D11Device> device);

    //  상수버퍼, 셰이더 바인딩
    void Render(ComPtr<ID3D11DeviceContext> context, const TransformData& data);

private:
    Shader _vs, _ps;
    ConstantBuffer<TransformData> _constantBuffer;
};