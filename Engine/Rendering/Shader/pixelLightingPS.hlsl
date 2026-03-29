/*==============================================================================

    ピクセルシェーダーで光源計算[piselLightingPS.hlsl]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/14
==============================================================================*/

#include "common.hlsl"

Texture2D g_Texture : register(t0);          //テクスチャ0番
SamplerState g_SamplerState : register(s0);  //サンプラー0番

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //ピクセルの法線を正規化
    float4 normal = normalize(In.Normal);
    //光源の計算
    float light = 0.5 - 0.5 * dot(Light.Direction.rgb, normal.xyz);
    
    //テクスチャのピクセル色を取得
    float4 baseColor;
    if (Material.TextureEnable)
    {
        baseColor = g_Texture.Sample(g_SamplerState, In.TexCoord);
    }
    else
    {
        baseColor = float4(1, 1, 1, 1); // ← 色はマテリアルに任せる（テクスチャの代替）
    }
    // 光の影響にマテリアルカラーを反映
    float3 lighting = In.Diffuse.rgb * light + Light.Ambient.rgb;
    outDiffuse.rgb = baseColor.rgb * lighting;
    outDiffuse.a = baseColor.a * In.Diffuse.a;
    
    //視線ベクトル
    //カメラからピクセルへ向かうベクトル
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev); //正規化
    
    //光の反射ベクトル
    float3 refv = reflect(Light.Direction.rgb, normal.xyz);
    refv = normalize(refv); //正規化
    
    //鏡面反射の計算
    float4 specular = -dot(eyev, refv); 
    specular = saturate(specular);      //値をサチュレート
    specular = pow(specular, 30);       //ここでは30乗してみる
    
    outDiffuse.rgb += specular;         //スペキュラー値をデフューずとして足しこむ
    
}