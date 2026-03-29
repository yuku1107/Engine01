/*==============================================================================

	レンダラー[renderer.h]
                                                             Author:郭　哲宇
															 Date  :2025/04/23
==============================================================================*/
#ifndef RENDERER_H
#define RENDERER_H

class Camera;

struct VERTEX_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};



struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};



struct LIGHT
{
	BOOL		Enable;          //フラグ
	BOOL		Dummy[3];
	XMFLOAT4	Direction;       //方向ベクトル
	XMFLOAT4	Diffuse;         //色
	XMFLOAT4	Ambient;         //環境光

	XMFLOAT4	Position;        //位置
	XMFLOAT4	PointLightParam; //距離減衰

	XMFLOAT4    SkyColor;        // 空の色
	XMFLOAT4    GroundColor;     // 地面の色
	XMFLOAT4    GroundNormal;    // 地面の法線ベクトル
	XMFLOAT4    Angle;           // 角度

	XMMATRIX    ViewMatrix;           //ライトカメラ用
	XMMATRIX    ProjectionMatrix;     //ライトカメラ用
};


//描画管理
class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*            m_CameraBuffer;
	static ID3D11Buffer*            m_ParameterBuffer;
	static ID3D11Buffer*            m_ReflectionBuffer;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;
	static ID3D11DepthStencilState* m_StencilWrite;
	static ID3D11DepthStencilState* m_StencilRead;

	static ID3D11RasterizerState*   m_RasterStateSolid;
	static ID3D11RasterizerState*   m_RasterStateWireframe;
	static ID3D11RasterizerState*   m_RasterStateCallBack;
	static ID3D11RasterizerState*   m_RasterStateCallNone;

	static ID3D11BlendState*		m_BlendState;
	static ID3D11BlendState*        m_BlendStateFalse;
	static ID3D11BlendState*		m_BlendStateATC;
	static ID3D11BlendState*        m_BlendStateAdd;
	static ID3D11BlendState*        m_BlendStateMask;


	//レンダリングテクスチャ関連
	static ID3D11RenderTargetView*   m_PERenderTargetView[2];
	static ID3D11DepthStencilView*   m_PEDepthStencilView[2];
	static ID3D11ShaderResourceView* m_PEShaderResourceView[2];
	static ID3D11Texture2D*          m_PERenderTargetTexture[2];

	//シャドウ
	static ID3D11DepthStencilView*   m_DepthShadowStencilView;
	static ID3D11ShaderResourceView* m_DepthShadowShaderResourceView;

	//反射用レンダリングテクスチャ
	static ID3D11RenderTargetView* m_RERenderTargetView;
	static ID3D11DepthStencilView* m_REDepthStencilView;
	static ID3D11ShaderResourceView* m_REShaderResourceView;
	static ID3D11Texture2D* m_RERenderTargetTexture;
	static XMFLOAT4X4 m_ReflectionViewProjFloat4x4;


public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void BeginPE(int n);
	static void End();

	static void BeginDepth();

	static void BeginReflection();
	static void UpdateReflectionViewProj(Camera* camera, const Vector3& planeNormal, const Vector3& planePoint);

	static void SetDepthEnable(bool Enable);
	static void SetBlendEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetAddBlendEnable(bool Enable);
	static void SetRasterSolid(bool Enable);

	static void SetStencilShadowWrite();
	static void SetStencilShadowRead();
	static void SetStencilShadowNone();

	static void ClearDepth();

	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(XMMATRIX WorldMatrix);
	static void SetViewMatrix(XMMATRIX ViewMatrix);
	static void SetProjectionMatrix(XMMATRIX ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetCameraPosition(XMFLOAT3 CameraPosition);
	static void SetParameter(XMFLOAT4 Parameter);
	static void SetReflection(XMFLOAT4X4 Reflection);
	
	//UV調整(Parameter利用)
	static void SetSpriteUV(float ox, float oy, float sx, float sy);

	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }
	static IDXGISwapChain* GetSwapChain(void) { return m_SwapChain; }
	static ID3D11RenderTargetView* GetRenderTargetView(void) { return m_RenderTargetView; }

	static ID3D11ShaderResourceView* GetPETexture(int n) { return m_PEShaderResourceView[n]; }
	static ID3D11ShaderResourceView* GetRETexture() { return m_REShaderResourceView; }
	static ID3D11ShaderResourceView* GetDepthStencil() { return m_DepthShadowShaderResourceView; }

	static XMFLOAT3 ScreenToWorld(int mouseX, int mouseY, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix);

	static void PrintGPUName(ID3D11Device* device);

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

};

#endif //RENDERER_H
