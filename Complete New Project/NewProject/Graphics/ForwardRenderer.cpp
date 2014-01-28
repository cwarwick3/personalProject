#include "ForwardRenderer.h"
#include "directx.h"
#include "mesh.h"
#include "shader.h"
#include <xnamath.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

#include "Camera.h"

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vLightDir[2];
	XMFLOAT4 vLightColor[2];
	XMFLOAT4 vOutputColor;
};

extern Mesh* pTestMesh;
extern Shader* pTestShader;

Camera* pTestCamera;
ID3D11Buffer*          g_pConstantBuffer = NULL;
XMMATRIX                g_World;
XMMATRIX                g_View;
XMMATRIX                g_Projection;

ID3D11PixelShader*      g_pPixelShaderSolid = NULL;

ForwardRenderer::ForwardRenderer() : Renderer("Forward") {
}

ForwardRenderer::~ForwardRenderer() {
    Renderer::~Renderer();
    if (pRenderTarget) {
        pRenderTarget->Release();
        pRenderTarget = NULL;
    }

    g_pPixelShaderSolid->Release();
    g_pConstantBuffer->Release();
}

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile2( const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}

HRESULT ForwardRenderer::init(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain) {
    // Create a render target view
    HRESULT hr = S_OK;
    pRenderTarget = NULL;
    
    RECT rc;
    GetClientRect( hwnd, &rc );
    mWidth = rc.right - rc.left;
    mHeight = rc.bottom - rc.top;

    ID3D11Texture2D* pBackBuffer = NULL;
    hr = pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = pDevice->CreateRenderTargetView( pBackBuffer, NULL, &pRenderTarget );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    pContext->OMSetRenderTargets( 1, &pRenderTarget, NULL );
    
    CreateDepthStencil(pDevice, pContext);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)mWidth;
    vp.Height = (FLOAT)mHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports( 1, &vp );
    
    // Create the constant buffer
    D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.StructureByteStride = 0;

    hr = pDevice->CreateBuffer( &bd, NULL, &g_pConstantBuffer );
    if( FAILED( hr ) )
        return hr;

    // Create test light shader
    // Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile2( "testShader.fx", "PSSolid", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK );
        return hr;
    }

	// Create the pixel shader
	hr = pDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShaderSolid );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;
    
    // Initialize the world matrix
	g_World = XMMatrixIdentity();

    // Initialize the view matrix
	XMVECTOR Eye = XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	g_View = XMMatrixLookAtLH( Eye, At, Up );

    // Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, mWidth / (FLOAT)mHeight, 0.01f, 100.0f );

    pTestCamera = new Camera();

    return S_OK;
}

HRESULT ForwardRenderer::CreateDepthStencil(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) {
    HRESULT hr = S_OK;
    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = mWidth;
    descDepth.Height = mHeight;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = pDevice->CreateTexture2D( &descDepth, NULL, &mDepthStencil );
    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    descDSV.Flags = 0;
    hr = pDevice->CreateDepthStencilView( mDepthStencil, &descDSV, &mDepthStencilView );
    if( FAILED( hr ) )
        return hr;

    pContext->OMSetRenderTargets( 1, &pRenderTarget, mDepthStencilView );

    return hr;
}

// Where the main rendering occurs
void ForwardRenderer::render(float dt, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain) {
    if (!pRenderTarget) {
        return;
    }
    
    // Update our time
    static float t = 0.0f;
    if( g_driverType == D3D_DRIVER_TYPE_REFERENCE )
    {
        t += ( float )XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if( dwTimeStart == 0 )
            dwTimeStart = dwTimeCur;
        t = ( dwTimeCur - dwTimeStart ) / 1000.0f;
    }

    // Rotate cube around the origin
	g_World = XMMatrixRotationY( t );

    // Setup our lighting parameters
    XMFLOAT4 vLightDirs[2] =
    {
        XMFLOAT4( -0.577f, 0.577f, -0.577f, 1.0f ),
        XMFLOAT4( 0.0f, 0.0f, -1.0f, 1.0f ),
    };
    XMFLOAT4 vLightColors[2] =
    {
        XMFLOAT4( 0.5f, 0.7f, 0.5f, 1.0f ),
        XMFLOAT4( 0.7f, 0.0f, 0.0f, 1.0f )
    };

    // Rotate the second light around the origin
	XMMATRIX mRotate = XMMatrixRotationY( -2.0f * t );
	XMVECTOR vLightDir = XMLoadFloat4( &vLightDirs[1] );
	vLightDir = XMVector3Transform( vLightDir, mRotate );
	XMStoreFloat4( &vLightDirs[1], vLightDir );

    //
    // Animate the cube
    //
    // TODO Replace this withe the object's world matrix
	g_World = XMMatrixRotationY( t );

    //
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    pContext->ClearRenderTargetView( pRenderTarget, ClearColor );
    
    //
    // Clear the depth buffer to 1.0 (max depth)
    //
    pContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

    //
    // Update variables
    //
    XMMATRIX cameraMatrix = XMLoadFloat4x4(&pTestCamera->updateCamera(dt));

    ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose( g_World );
	cb.mView = XMMatrixTranspose( cameraMatrix );
	cb.mProjection = XMMatrixTranspose( g_Projection );
	cb.vLightDir[0] = vLightDirs[0];
	cb.vLightDir[1] = vLightDirs[1];
	cb.vLightColor[0] = vLightColors[0];
	cb.vLightColor[1] = vLightColors[1];
	cb.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	pContext->UpdateSubresource( (g_pConstantBuffer), 0, NULL, &cb, 0, 0 );

    pTestShader->prepareToDraw(pContext,(g_pConstantBuffer));

    pTestMesh->SetAndDraw(pContext);

    //
    // Render each light
    //
    for( int m = 0; m < 2; m++ )
    {
		XMMATRIX mLight = XMMatrixTranslationFromVector( 5.0f * XMLoadFloat4( &vLightDirs[m] ) );
		XMMATRIX mLightScale = XMMatrixScaling( 0.2f, 0.2f, 0.2f );
        mLight = mLightScale * mLight;

        // Update the world variable to reflect the current light
		cb.mWorld = XMMatrixTranspose( mLight );
		cb.vOutputColor = vLightColors[m];
		pContext->UpdateSubresource( g_pConstantBuffer, 0, NULL, &cb, 0, 0 );

		pContext->PSSetShader( g_pPixelShaderSolid, NULL, 0 );
        pTestMesh->SetAndDraw(pContext);
    }
    
    //
    // Present our back buffer to our front buffer
    //
    pSwapChain->Present( 0, 0 );
}
