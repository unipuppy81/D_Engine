#pragma once
#include <wrl/client.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

using Microsoft::WRL::ComPtr;

class RasterizerState {
public:
    RasterizerState() = default;
    ~RasterizerState() = default;

    /**
     * @brief 래스터라이저 상태를 생성합니다.
     * @param fillMode 면 채우기 방식 (D3D11_FILL_SOLID 등)
     * @param cullMode 뒷면 제거 방식 (D3D11_CULL_BACK 등)
     */
    void Create(ComPtr<ID3D11Device> device, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode) {
        D3D11_RASTERIZER_DESC rd = {};
        rd.FillMode = fillMode;
        rd.CullMode = cullMode;
        rd.FrontCounterClockwise = FALSE; // 시계 방향(CW)을 앞면으로 판단

        device->CreateRasterizerState(&rd, _rasterizerState.GetAddressOf());
    }

    void Bind(ComPtr<ID3D11DeviceContext> context) {
        context->RSSetState(_rasterizerState.Get());
    }

private:
    ComPtr<ID3D11RasterizerState> _rasterizerState;
};