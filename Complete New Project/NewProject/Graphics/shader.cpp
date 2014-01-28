#include "shader.h"
#include "directx.h"
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "VertexTypes.h"

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
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

Shader::Shader(const char* filename, ID3D11Device* pDevice, ID3D11DeviceContext* pContext) {
    HRESULT hr;
    mVertexShader = 0;
    mPixelShader = 0; 
    mVertexLayout = 0;

    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    hr = CompileShaderFromFile( filename, "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK );
        return;
    }

	// Create the vertex shader
	hr = pDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &mVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
        return;
	}
    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
	UINT numElements = ARRAYSIZE( simpleLayout );

    // Create the input layout
	hr = pDevice->CreateInputLayout( simpleLayout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &mVertexLayout );
	pVSBlob->Release();
	if( FAILED( hr ) )
        return;

    // Set the input layout
    pContext->IASetInputLayout( mVertexLayout );

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( filename, "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK );
        return;
    }

	// Create the pixel shader
	hr = pDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &mPixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return;
}

Shader::~Shader() {
    if (mVertexShader) mVertexShader->Release();
    if (mPixelShader) mPixelShader->Release();
    if (mVertexLayout) mPixelShader->Release();
}

void Shader::prepareToDraw(ID3D11DeviceContext* pContext, ID3D11Buffer* constantBuffer) {
    if (mVertexShader) {
        pContext->VSSetShader(mVertexShader, NULL, 0);
    }

    if (constantBuffer) {
        pContext->VSSetConstantBuffers(0, 1, &constantBuffer);
    }

    if (mPixelShader) {
        pContext->PSSetShader(mPixelShader, NULL, 0);
    }

    if (constantBuffer) {
        pContext->PSSetConstantBuffers(0, 1, &constantBuffer);
    }

    pContext->IASetInputLayout(mVertexLayout);
}
