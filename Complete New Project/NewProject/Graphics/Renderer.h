#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include "D3D11.h"

class Renderer {
public:
    Renderer() : mName("") {}
    Renderer(std::string name) : mName(name) {}
    virtual ~Renderer() {}
    
    virtual HRESULT init(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain) = 0;
    virtual void render(float dt, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain) = 0;

    virtual HRESULT onDeviceLost() = 0;
    virtual HRESULT onDeviceReset() = 0;
protected:
    std::string mName;
    ID3D11RenderTargetView* pRenderTarget;
};

#endif
