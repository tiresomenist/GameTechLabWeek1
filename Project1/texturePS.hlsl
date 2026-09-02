
Texture2D MyTexture : register(t0); // The texture resource data
SamplerState MySampler : register(s0); // The state defining filtering & wrapping

// 상수버퍼
cbuffer PixelConstants : register(b0)
{
    // 피격 시 흰색 깜빡임 정도 (0.0 ~ 1.0)
    float hitFlashAmount;
    float3 padding;
};

struct PS_INPUT
{
    float4 posProj : SV_POSITION;
    float4 posWord : POSITION;
    float4 col : COLOR;
    float2 uv : TEXCOORD;
};

                                   
float4 mainPS(PS_INPUT input) : SV_TARGET
{
    float4 texColor = MyTexture.Sample(MySampler, input.uv);
    
    // 피격 시 흰색 깜빡임
    float3 hitColor = float3(1.0, 0.0, 0.0);
    texColor.rgb = lerp(
        texColor.rgb,
        hitColor,
        hitFlashAmount
    );
    return texColor;
}