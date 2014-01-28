#ifndef FORWARDRENDERER_H
#define FORWARDRENDERER_H

#include "Renderer.h"
#include "D3D11.h"

class ForwardRenderer : public Renderer {
public:
    ForwardRenderer();
    ~ForwardRenderer();
    
    HRESULT init(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain);

    void render(float dt, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain);
    
    HRESULT onDeviceLost() { return S_OK; }
    HRESULT onDeviceReset() { return S_OK; }
private:
    unsigned int mWidth;
    unsigned int mHeight;
    
    ID3D11Texture2D*        mDepthStencil;
    ID3D11DepthStencilView* mDepthStencilView;

    HRESULT CreateDepthStencil(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
};

#endif
