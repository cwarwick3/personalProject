#include "mesh.h"
#include "VertexTypes.h"
#include <string>
#include <sstream>

Mesh::Mesh() {
    mVertexBuffer = 0;
    mIndexBuffer = 0;
    mNumVertices = 0;
    mNumIndices = 0;
    mNumTris = 0;
}

Mesh::~Mesh() {
    if (mVertexBuffer) mVertexBuffer->Release();
    if (mIndexBuffer) mIndexBuffer->Release();
}

// init
// Takes in pointers to vertex and index data used to create the mesh's buffers
HRESULT Mesh::init(ID3D11Device* pDevice, void* pVertices, unsigned int numVerts, WORD* pIndices, unsigned int numIndices) {
    if (!pVertices || !pIndices || numVerts <= 0 || numIndices <= 0) {
        return -1;
    }

    mNumVertices = numVerts;
    mNumIndices = numIndices;
    mNumTris = mNumIndices / 3;
    
    D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( simpleVertex ) * mNumVertices;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = pVertices;
    HRESULT hr = pDevice->CreateBuffer( &bd, &InitData, &mVertexBuffer );
    if( FAILED( hr ) )
        return hr;
    mStride = sizeof(simpleVertex);

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( WORD ) * mNumIndices;        // 36 vertices needed for 12 triangles in a triangle list
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
    InitData.pSysMem = pIndices;
    hr = pDevice->CreateBuffer( &bd, &InitData, &mIndexBuffer );
    if( FAILED( hr ) )
        return hr;

    return hr;
};

// Use the given device context to render the mesh
void Mesh::SetAndDraw(ID3D11DeviceContext* pContext) {
    if (!mVertexBuffer || !mIndexBuffer) {
        return;
    }
    
    UINT offset = 0;
    pContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &mStride, &offset);
    pContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pContext->DrawIndexed(mNumIndices, 0, 0);
}

std::string Mesh::dumpData() {
    std::stringstream sstm;
    std::string data;
    
    sstm << "Number of Vertices: " << mNumVertices << std::endl;
    sstm << "Number of Indices: " << mNumIndices << std::endl;
    sstm << "Number of Triangles: " << mNumTris << std::endl;
    
    data = sstm.str();
    return data;
}
