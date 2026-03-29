/*==============================================================================

    ピクセルシェーダー[vertexLightingPS.hlsl]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/13
==============================================================================*/

#include "common.hlsl"

Texture2D g_Texture : register(t0);         //テクスチャ0番
SamplerState g_SamplerState : register(s0); //サンプラー0番

void main (in PS_IN In,out float4 outDiffuse:SV_Target)
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
}