
#include "common.hlsl"

StructuredBuffer<float3> Time : register(t2);

void main(in VS_IN In, out PS_IN Out)
{
    // ã^éóäpìx
    float time = Time[0].x;

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    // --- UV âÒì] ---
    float2 uv = In.TexCoord - float2(0.5f, 0.5f);

    float s = sin(time);
    float c = cos(time);

    float2 rotUV;
    rotUV.x = uv.x * c - uv.y * s;
    rotUV.y = uv.x * s + uv.y * c;

    //0.6Å®(0.2Å`0.8)
    Out.TexCoord = rotUV * 0.6f + float2(0.5f, 0.5f);
    Out.Position = mul(In.Position, wvp);
    Out.Diffuse = In.Diffuse * Material.Diffuse;
    Out.Depth = Out.Position.z;
}


