#pragma once
#include "Shader.h"
#include "ConstantBuffer.h"
#include "StructData.h"

class ColorShader {
public:
    bool Initialize(ComPtr<ID3D11Device> device);

    //  상수버퍼, 셰이더 바인딩
    void Render(ComPtr<ID3D11DeviceContext> context, const TransformData& tData, const LightData& lData);

private:
    Shader _vs, _ps;
    ConstantBuffer<TransformData> _constantBuffer;      // b0 슬롯
    ConstantBuffer<LightData> _lightBuffer;             // b1 슬롯
};