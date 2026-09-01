//cbuffer VertexConstantBuffer : register(b0)
//{
//    matrix model;
//    matrix view;
//    matrix projection;
//};

cbuffer VertexConstantBuffer : register(b0)
{
    float3 Offset;
    float Radius;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 col : COLOR;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 posProj : SV_POSITION;
    float4 posWord : POSITION;
    float4 col : COLOR;
    float2 uv : TEXCOORD;
};


PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    output.posProj = float4(Offset, 0.0f) + float4(input.pos.xyz * Radius, 1.0f);
    output.col = input.col;
    
    output.uv = input.uv;
    
    //float4 pos = float4(input.pos, 1.0f);
    //pos = mul(pos, model);    
    //output.posWord = pos;
    
    //pos = mul(pos, view);
    //pos = mul(pos, projection);
    //output.posProj = pos;
    //output.col = input.col;
    return output;
}