cbuffer VertexConstantBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 col : COLOR;
};

struct PS_INPUT
{
    float4 posProj : SV_POSITION;
    float4 posWord : POSITION;
    float4 col : COLOR;
};


PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, model);
    output.posWord = pos;
    
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.posProj = pos;
    output.col = input.col;
    return output;
}