/*==============================================================================

    クックトランス[CookTorrancePS.hlsl]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/28
==============================================================================*/

#include "common.hlsl"

Texture2D g_Texture : register(t0);

SamplerState g_SamplerState : register(s0);

//パラメータ計算用ローカル関数のプロトタイプ宣言
float CaluculateBeckmann(float smooth, float nh);
float CaluculateFresnel(float metal, float hv);
float CaluculateGeometricDampint(float nh, float nv, float nl, float vh);

//ベックマン分布関数
float CaluculateBeckmann(float smooth, float nh)
{
    float D;
    
    D = 
    (
    exp(
         -(1 - (nh * nh)) / (smooth * smooth * nh * nh)
        )
         / (4 * smooth * smooth * nh * nh * nh * nh)
    );
    
    return D;
}

//フレネル反射項
float CaluculateFresnel(float metal,float hv)
{
    //Schlick近似metal値は0.0-1.0とする
    return metal + (1 - metal) * pow(1 - hv, 5);
}

//幾何減衰率
float CaluculateGeometricDamping(float nh,float nv,float nl,float vh)
{
    float G;
    G = min(1, min(2 * nh * nv / vh, 2 * nh * nl / vh));
    return G;
}

void main(in PS_IN In,out float4 outDiffuse:SV_Target)
{
    //物体から光源へのベクトル
    float4 lv = Light.Position - In.WorldPosition;
    //物体と光源の距離
    float4 ld = length(lv);
    //ベクトルを正規化
    lv = normalize(lv);
    
    //減衰の計算
    float ofs = 1.0 - (1.0 / Light.PointLightParam) * ld;
    //減衰率0未満は0にする
    ofs = max(0, ofs);
    
    //ピクセル法線を正規化
    float4 normal = normalize(In.Normal);
    
    //光源計算
    float light = 0.5 - 0.5 * dot(lv.xyz, normal.xyz);
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
    float3 eyev = CameraPosition.xyz - In.WorldPosition.xyz;
    eyev = normalize(eyev);
    
    //ハーフベクトルを計算
    float3 halfv = lv.xyz + eyev.xyz; //ライトベクトル + 視線ベクトル
    halfv = normalize(halfv);
    
    //スペキュラーの計算
    float3 specular;
    float nv = saturate(dot(normal.xyz, eyev));
    float nh = saturate(dot(normal.xyz, halfv));
    float vh = saturate(dot(eyev, halfv));
    float nl = saturate(dot(normal.xyz, lv.xyz));
    
    float D = CaluculateBeckmann(0.3f, nh); //面の粗さ
    float G = CaluculateGeometricDamping(nh, nv, nl, vh);
    float F = CaluculateFresnel(0.8f, dot(lv.xyz, halfv));//金属っぽさ
    
    specular = baseColor.rgb * max(0.0f, F * D * G / nv);

    outDiffuse.rgb += (specular * ofs);   //スペキュラーも減衰させてから加算して出力

}