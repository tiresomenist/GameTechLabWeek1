#include "D3D11Util.h"
#include "App.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

void D3D11Util::CreateVertexShaderAndInputLayout(
    const LPCWSTR& fileName,
    ID3D11Device* device,
    ID3D11VertexShader** vertexShader,
    ID3D11InputLayout** inputLayout)
{
    ID3DBlob* vertexshaderCSO = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr = D3DCompileFromFile(fileName, nullptr, nullptr,
        "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, &errorBlob);

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        return;
    }

    device->CreateVertexShader(
        vertexshaderCSO->GetBufferPointer(),
        vertexshaderCSO->GetBufferSize(),
        nullptr,
        vertexShader);         

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    device->CreateInputLayout(inputElementDesc,
        ARRAYSIZE(inputElementDesc),
        vertexshaderCSO->GetBufferPointer(),
        vertexshaderCSO->GetBufferSize(),
        inputLayout);          

    vertexshaderCSO->Release();
}

void D3D11Util::CreatePixelShader(const LPCWSTR& fileName, ID3D11PixelShader** pixelShader)
{
    ID3DBlob* pixelshaderCSO = nullptr;

    HRESULT hr = D3DCompileFromFile(fileName, nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);



    App::Ins->GetDevice()->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, pixelShader);
    pixelshaderCSO->Release();

}



