/*==============================================================================

    ピクセルシェーダ＿グレースケール[grayscalPS.hlsl]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/07
==============================================================================*/

#include "common.hlsl" 

Texture2D g_Texture : register(t0); 

SamplerState g_SamplerState : register(s0); 

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
    
    
    
    //outDiffuse.rbg = 1.0f - outDiffuse.rbg; //ネガ反転
    //outDiffuse.rgb *= outDiffuse.rgb; //
    //return;
    
    
    //グレースケール処理
    outDiffuse.rgb = outDiffuse.r * 0.299f + outDiffuse.g * 0.587f + outDiffuse.b * 0.114f;
    
    //セピア変換
    float4 sepia = float4(1.0f, 0.0f, 0.0f, 1.0f); //ベースになる色
    outDiffuse *= sepia;

}

