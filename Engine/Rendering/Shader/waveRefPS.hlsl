#include "common.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_RefTexture : register(t1);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

    float3 worldPos = In.WorldPosition.xyz;
    float3 viewDir = normalize(CameraPosition.xyz - worldPos);
    float3 normal = normalize(In.Normal.xyz);

    // 反射テクスチャを「反射カメラの ViewProj で投影して得たUV」でサンプリング 
    float4 reflClip = mul(float4(worldPos, 1.0f), ReflectionViewProj); 
    reflClip /= reflClip.w;

    float2 reflUV = reflClip.xy * 0.5f + 0.5f;

    // レンダーターゲット座標系とサンプリング座標系が上下逆ので反転する
    reflUV.y = 1.0f - reflUV.y;
    reflUV = clamp(reflUV, 0.0f, 1.0f);

    float4 reflectionColor = g_RefTexture.Sample(g_SamplerState, reflUV) * In.Diffuse;

    if (Material.TextureEnable)
    {
        //float4 baseColor = g_Texture.Sample(g_SamplerState, In.TexCoord) * In.Diffuse;
        float f0 = 0.3f;
        float d = saturate(dot(viewDir, normal));
        float fresnel = f0 + (1.0f - f0) * pow(1.0f - d, 5.0f);
        outDiffuse = reflectionColor;
        outDiffuse.a = fresnel;
    }
    else
    {
        // テクスチャ無し：Diffuse と反射の合成
        float f0 = 0.3f;
        float d = saturate(dot(viewDir, normal));
        float fresnel = f0 + (1.0f - f0) * pow(1.0f - d, 5.0f);

        outDiffuse = lerp(In.Diffuse, reflectionColor * In.Diffuse, fresnel);
        outDiffuse.a = fresnel;
    }
    
}
