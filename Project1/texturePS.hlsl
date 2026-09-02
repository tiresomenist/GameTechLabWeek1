
Texture2D MyTexture : register(t0); // The texture resource data
SamplerState MySampler : register(s0); // The state defining filtering & wrapping

struct PS_INPUT
{
    float4 posProj : SV_POSITION;
    float4 posWord : POSITION;
    float4 col : COLOR;
    float2 uv : TEXCOORD;
};

                                   
float4 mainPS(PS_INPUT input) : SV_TARGET
{
    return MyTexture.Sample(MySampler, input.uv);
}