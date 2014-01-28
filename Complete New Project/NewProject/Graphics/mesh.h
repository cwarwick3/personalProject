#ifndef MESH_H
#define MESH_H

#include "D3D11.h"
#include "directx.h"
#include <string>

class Mesh {
public:
    Mesh();
    ~Mesh();

    HRESULT init(ID3D11Device* pDevice, void* pVertices, unsigned int numVerts, WORD* pIndices, unsigned int numIndices);

    void SetAndDraw(ID3D11DeviceContext* pContext);

    std::string dumpData();
private:
    ID3D11Buffer* mVertexBuffer;
    ID3D11Buffer* mIndexBuffer;
    UINT         mStride;
    unsigned int mNumVertices;
    unsigned int mNumIndices;
    unsigned int mNumTris;
};

#endif
