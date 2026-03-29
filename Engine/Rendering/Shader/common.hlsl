


cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}





struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	bool TextureEnable;
	float2 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}




struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
	
    float4 Position;        //光の位置
    float4 PointLightParam; //光の届く距離

    float4 SkyColor;        //空の色
    float4 GroundColor;     //地面の色
    float4 GroundNormal;    //地面の法線ベクトル
    float4 Angle;           //角度
	
    matrix ViewMatrix;        //ライトカメラ用
    matrix ProjectionMatrix;  //ライトカメラ用
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}

cbuffer CameraBuffer : register(b5) // バッファの5番とする
{
    float4 CameraPosition; //カメラ座標を受け取る変数
};

cbuffer ParameterBuffer : register(b6)
{
    float4 Parameter; //シェーダー内で使う変数名
};

cbuffer ReflectionBuffer : register(b7)
{
    float4x4 ReflectionViewProj;
};

struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
    uint InstanceId     : SV_InstanceID;
};


struct PS_IN
{
    float4 Position      : SV_POSITION;
    float4 Normal        : NORMAL0;
    float4 Diffuse       : COLOR0;
    float2 TexCoord      : TEXCOORD0;
    float4 WorldPosition : POSITION0;
    float Depth          : DEPTH0;
    float4 LightPosition : POSITION1;
};
