
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
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
    
    //フェード
    outDiffuse.rgb = lerp(outDiffuse.rgb, float3(0, 0, 0), Parameter.w);
    
    
    //赤枠
    float2 uv = In.TexCoord;

    //画面端までの距離
    float edge = min(min(uv.x, 1.0 - uv.x),min(uv.y, 1.0 - uv.y));
    
    float frameMask = smoothstep(0.2, 0.0, edge);

    //HP
    float hp = saturate(Parameter.x);

    //脈打ち
    float pulseSpeed = lerp(2.0, 8.0, hp);
    float pulse = sin(Parameter.y * pulseSpeed) * 0.5 + 0.5;
    float pulsePower = lerp(0.2, 0.8, hp);

    //最終強度
    float intensity = frameMask * hp * (0.6 + pulse * pulsePower);

    //赤くブレンド
    outDiffuse.rgb = lerp(outDiffuse.rgb, float3(1.0, 0.0, 0.0), intensity);
    
}
