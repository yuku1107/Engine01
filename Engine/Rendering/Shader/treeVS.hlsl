
#include "common.hlsl"

StructuredBuffer<float3> Position :register(t2);
StructuredBuffer<float3> Scale : register(t3);
StructuredBuffer<float3> Time : register(t4);

void main(in VS_IN In, out PS_IN Out)
{
    float3 instancePos = Position[In.InstanceId];
    float2 instanceScale = Scale[In.InstanceId];
    float time = Time[0].x;
    
    float3 pos = In.Position.xyz;
    pos.x *= instanceScale.x;
    pos.y *= instanceScale.y;

    // 風アニメーション
    float windPower = 0.1; // 揺れる強さ
    float windSpeed = 1.5; // 揺れる速さ

    float maxHeight = instanceScale.y;

    // 木の高さを0～1に正規化（根元＝0、先端＝1）
    float heightFactor = saturate(pos.y / maxHeight);

    // 揺れ量
    float windOffset =sin(time * windSpeed + instancePos.x * 0.2 + instancePos.z * 0.2)* windPower * heightFactor;

    // X方向に揺らす
    pos.x += windOffset;

    // ビルボード処理 (Y軸固定)
    float3 right = normalize(float3(View._11, View._21, View._31));
    float3 up = float3(0, 1, 0);
    float3 worldPos = right * pos.x + up * pos.y + instancePos;

    Out.Position = mul(float4(worldPos, 1.0f), View);
    Out.Position = mul(Out.Position, Projection);

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse;
}
