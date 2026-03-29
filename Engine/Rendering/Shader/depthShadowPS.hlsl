
#include "common.hlsl"

Texture2D g_Texture : register(t0);            //通常テクスチャ
Texture2D g_TextureShadowDepth : register(t1); //シャドウマップ
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

    //日向での色＝普通のテクスチャ*頂点色を作成しておく
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;

    In.LightPosition.xyz /= In.LightPosition.w;//正規化デバイス座標を作る
    In.LightPosition.x = In.LightPosition.x * 0.5f + 0.5f; //テクスチャ座標に変換
    In.LightPosition.y = -In.LightPosition.y * 0.5f + 0.5f;
   
    // NDCの範囲チェック
    bool outside =
    In.LightPosition.x < 0.0f || In.LightPosition.x > 1.0f ||
    In.LightPosition.y < 0.0f || In.LightPosition.y > 1.0f ||
    In.LightPosition.z < 0.0f || In.LightPosition.z > 1.0f;

    if (outside)
    {
        outDiffuse.rgb = outDiffuse.rgb; //影なし
        return;
    }
    
    //PCFテスト
    float2 offset = float2(1.0f / 1280.0f, 1.0f / 720.0f);
    float shadowRate = 0.0f;
    float depth[4];
    depth[0] = g_TextureShadowDepth.Sample(g_SamplerState, In.LightPosition.xy);
    depth[1] = g_TextureShadowDepth.Sample(g_SamplerState, In.LightPosition.xy + float2(offset.x, offset.y));
    depth[2] = g_TextureShadowDepth.Sample(g_SamplerState, In.LightPosition.xy + float2(offset.x, 0.0f));
    depth[3] = g_TextureShadowDepth.Sample(g_SamplerState, In.LightPosition.xy + float2(0.0f, offset.y));
    
    for (int i = 0; i < 4;i++)
    {
        if ((In.LightPosition.z - 0.01f) > depth[i])
        {
            shadowRate++;//影になるピクセルの数
        }
    }
    
    shadowRate /= 4.0f;
    float3 shadowColor = outDiffuse.rgb * 0.5f;
    shadowColor = lerp(outDiffuse.rgb, shadowColor, shadowRate);
    outDiffuse.rgb = shadowColor;


}
