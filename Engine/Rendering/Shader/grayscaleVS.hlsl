/*==============================================================================

    頂点シェーダ＿グレースケール[grayscalVS.hlsl]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/07
==============================================================================*/

#include "common.hlsl"



void main(in VS_IN In, out PS_IN Out)
{

    
    matrix wvp; 
    wvp = mul(World, View);
    wvp = mul(wvp, Projection); 
    

    Out.Position = mul(In.Position, wvp); 
    
    Out.Normal = In.Normal;
    Out.TexCoord = In.TexCoord; 
    Out.Diffuse = In.Diffuse; 
}

