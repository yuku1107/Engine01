/*==============================================================================

    ポイントライティング[spotLightingPS.hlsl]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/03
==============================================================================*/

#include "common.hlsl"

Texture2D g_Texture : register(t0);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In,out float4 outDiffuse:SV_Target)
{
    //光源からピクセルへのベクトル
    float3 lv = normalize(In.WorldPosition.xyz - Light.Position.xyz);
    
    //光源計算
    float4 normal = normalize(In.Normal);
    float light = 0.5 - 0.5 * dot(lv, normal.xyz);
    light = saturate(light);
    
    //距離による減衰
    float3 ld = length(In.WorldPosition - Light.Position);
    float ofs = 1.0f - (1.0f / Light.PointLightParam.x) * ld;
    ofs = saturate(ofs);
    light *= ofs;
    
    //視線ベクトル
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    //反射ベクトル
    float3 refv = reflect(normalize(Light.Direction.xyz), normal.xyz);
    refv = normalize(refv);
    //float3 halfv = lv + eyev.xyz;
    //halfv = normalize(halfv);
    
    //スペキュラーの計算
    float specular = -dot(eyev, refv);
    //float specular = -dot(normal.xyz, halfv);
    specular = saturate(specular);
    specular = pow(specular, 30.0f);
    
    //コーンの向きとlvの角度
    float angle = acos(dot(lv.xyz, normalize(Light.Direction.xyz)));
    
    //angleがコーンの中にどのくらい入っているか？による明るさ値
    float spot = 1.0f - (1.0f / Light.Angle.x) * abs(angle);
    spot *= Light.PointLightParam.y;          //明るさ補正値
    
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
    
}