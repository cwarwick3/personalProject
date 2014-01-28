#ifndef DIRECTX_H
#define DIRECTX_H

#include "D3D11.h"

static D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
static D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
static ID3D11Device*           g_pd3dDevice = NULL;
static ID3D11DeviceContext*    g_pImmediateContext = NULL;
static IDXGISwapChain*         g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_pRenderTargetView = NULL;

HRESULT InitDevice(HWND hwnd, ID3D11Device** pDevice, ID3D11DeviceContext** pContext, IDXGISwapChain** pSwapChain);
void CleanupDevice();

#endif
