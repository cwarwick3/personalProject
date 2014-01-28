#ifndef VERTEXTYPES_H
#define VERTEXTYPES_H

#include "D3D11.h"
#include <xnamath.h>

struct simpleVertex {
    XMFLOAT3 Pos;
    XMFLOAT3 Normal;
};

// Define the input layout
static D3D11_INPUT_ELEMENT_DESC simpleLayout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

#endif
