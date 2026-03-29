/*==============================================================================

    ポイントライティング[pointLightingBlinnPhongPS.hlsl]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/28
==============================================================================*/

#include "common.hlsl"

Texture2D g_Texture : register(t0);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In,out float4 outDiffuse:SV_Target)
{
    //光源からピクセルへのベクトル
    float4 lv = In.WorldPosition - Light.Position;
    //物体と光源の距離
    float4 lb = length(lv);
    //ベクトルを正規化
    lv = normalize(lv);
    
    //減衰の計算
    float ofs = 1.0 - (1.0 / Light.PointLightParam) * lb;
    //減衰率0未満は0にする
    ofs = max(0, ofs);
    
    //ピクセル法線を正規化
    float4 normal = normalize(In.Normal);
    //光源計算
    float light = 0.5 - 0.5 * dot(lv.xyz, normal.xyz);
    light = saturate(light); //値をサチュレート
    light *= ofs; //明るさを減衰率させる
    
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
    
    //カメラからピクセルへ向かうベクトル
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    //ハーフベクトルを計算
    float3 halfv = lv.xyz + eyev.xyz; //ライトベクトル + 視線ベクトル
    halfv = normalize(halfv);
    
    //スペキュラーの計算
    float specular = -dot(normal.xyz, halfv);  //ハーフベクトルと法線の内積
    specular = saturate(specular);        
    specular = pow(specular, 30);         
                                          
    outDiffuse.rgb += (specular * ofs);   //スペキュラーも減衰させてから加算して出力

}