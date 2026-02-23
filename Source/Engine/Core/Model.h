#pragma once
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

using Microsoft::WRL::ComPtr;

class Model {
public:
    Model() = default;
    ~Model() = default;

    bool Initialize(ComPtr<ID3D11Device> device);
    void Shutdown();
    void Render(ComPtr<ID3D11DeviceContext> context);

    int GetIndexCount() { return _indexCount; }

private:
    bool InitializeBuffers(ComPtr<ID3D11Device> device);

    VertexBuffer _vertexBuffer;
    IndexBuffer _indexBuffer;
    int _indexCount = 0;
};