#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "D3D11.h"

class Shader {
public:
    Shader() : mVertexShader(NULL), mPixelShader(NULL), mShaderFile("") {}
    Shader(const char* filename, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    ~Shader();

    void prepareToDraw(ID3D11DeviceContext* pContext, ID3D11Buffer* constantBuffer);
private:
    ID3D11VertexShader* mVertexShader;
    ID3D11PixelShader* mPixelShader;
    ID3D11InputLayout* mVertexLayout;
    std::string mShaderFile;
};

#endif 