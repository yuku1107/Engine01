#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp;
	wvp = mul(World, View);		
	wvp = mul(wvp, Projection);	

	//頂点法線をワールド行列で変換して出力
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0f);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
    
    //光源処理
    float light = -dot(Light.Direction.xyz, worldNormal.xyz);
    light = saturate(light);
    Out.Diffuse = light;
    Out.Diffuse.a = In.Diffuse.a;
    
    Out.Position = mul(In.Position, wvp);
    Out.Normal = worldNormal;
    Out.TexCoord = In.TexCoord;

    matrix lightwvp; //ライトカメラのワールドビュープロジェクション行列
    lightwvp = mul(World, Light.ViewMatrix);
    lightwvp = mul(lightwvp, Light.ProjectionMatrix);
    Out.LightPosition = mul(In.Position, lightwvp);
}
