/*==============================================================================

    フィールド[fieldPS.hlsl]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/13
==============================================================================*/

#include "common.hlsl"

Texture2D g_Texture : register(t0);         
Texture2D g_TextureSoil : register(t1);
SamplerState g_SamplerState : register(s0); 

void main (in PS_IN In,out float4 outDiffuse:SV_Target)
{
    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord) * In.Diffuse.a;
        outDiffuse += g_TextureSoil.Sample(g_SamplerState, In.TexCoord) * (1.0f - In.Diffuse.a);
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }
    
    //高さフォグ
    float3 heightFogColor = float3(0.0f, 0.1f, 0.2f);
    float heightFog = saturate(-In.WorldPosition.y * 0.5f);
    outDiffuse.rgb = outDiffuse.rgb * (1.0f - heightFog) + heightFogColor * heightFog;
    
    
    outDiffuse.a = 1.0f;
}