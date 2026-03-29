
#include "common.hlsl"

StructuredBuffer<float3> Position :register(t2);
StructuredBuffer<float3> Scale : register(t3);

void main(in VS_IN In, out PS_IN Out)
{
    float3 instancePos = Position[In.InstanceId];
    float3 instanceScale = Scale[In.InstanceId];
    
    float3 pos = In.Position.xyz;
    pos.x *= instanceScale.x;
    pos.y *= instanceScale.y;
    
    // ビルボード処理
    float3 right = normalize(float3(View._11, View._21, View._31));
    float3 up = normalize(float3(View._12, View._22, View._32));
    float3 worldPos = right * pos.x + up * pos.y + instancePos;

    Out.Position = mul(float4(worldPos, 1.0f), View);
    Out.Position = mul(Out.Position, Projection);

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;

}

