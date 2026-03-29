
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

    if (Material.TextureEnable)
    {
        float2 uv = In.TexCoord * Parameter.zw + Parameter.xy;
        outDiffuse = g_Texture.Sample(g_SamplerState, uv);
        
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }
    
    if (outDiffuse.a < 0.01f)
    {
        discard;
    }
    
}
