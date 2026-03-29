/*==============================================================================

    リムライティング[limLightingVS.hlsl]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/04
==============================================================================*/

#include "common.hlsl"

void main(in VS_IN In,out PS_IN Out)
{
    //頂点変換、頂点座標出力
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, wvp);
    
    //頂点法線をワールド行列で回転させる
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0f);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
    Out.Normal = worldNormal;
    
    //頂点のUV座標と色をそのまま出力
    Out.TexCoord = In.TexCoord;
    if (Material.TextureEnable)
    {
        Out.Diffuse = In.Diffuse;
    }
    else
    {
        Out.Diffuse = In.Diffuse * Material.Diffuse;
    }
    
    //ワールド変換した頂点座標を出力（この行は最初追加しない）
    Out.WorldPosition = mul(In.Position, World);
}