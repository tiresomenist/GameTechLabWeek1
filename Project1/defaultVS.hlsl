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
    float3 Rotation;
    float Padding;
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


float3 ApplyRotation(float3 pos, float3 rot)
{
    
    float3 p = pos;
    float sx = sin(rot.x);
    float sy = sin(rot.y);
    float sz = sin(rot.z);
    float cx = cos(rot.x);
    float cy = cos(rot.y);
    float cz = cos(rot.z);
    
    //z축 회전
    float xz = p.x * cz - p.y * sz;
    float yz = p.x * sz + p.y * cz;
    p.x = xz;
    p.y = yz;
    //x축 회전
    float yx = p.y * cx - p.z * sx;
    float zx = p.y * sx + p.z * cx;
    p.y = yx;
    p.z = zx;
    //y축 회전
    float xy = p.x * cy + p.z * sy;
    float zy = -p.x * sy + p.z * cy;
    p.x = xy;
    p.z = zy;
    
    return p;
}

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    output.posProj = float4(Offset, 0.0f) + float4(ApplyRotation(input.pos.xyz, Rotation) * Radius, 1.0f);
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