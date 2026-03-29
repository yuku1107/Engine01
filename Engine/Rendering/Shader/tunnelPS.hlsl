
#include "common.hlsl"

Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float time = Parameter.x;
    float speed = 0.02f;
    float2 uv = In.TexCoord;
    
    //中心基準
    float2 p = uv - float2(0.5f, 0.5f);

    //半径
    float radius = length(p);

    //奥へ進む
    float depth = frac(radius - time * speed);

    //ドーナツ状サンプリング設定
    float texMinR = 0.2f;  //中心を使わない
    float texMaxR = 0.3f;  //外周
    
    float texR = lerp(texMinR, texMaxR, depth);
    
    //方向
    float2 dir = (radius > 0.001f) ? normalize(p) : float2(0.0f, 0.0f);
    
    float2 newUV = dir * texR + float2(0.5f, 0.5f);

    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, newUV);
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }
    
    if (outDiffuse.a < 0.01f)
    {
        discard;
    }
    
}
