/*==============================================================================

	レンダラー[renderer.cpp]
															 Author:郭　哲宇
															 Date  :2025/04/23
==============================================================================*/
#include "main.h"
#include <io.h>
#include <iostream>
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Objects/SceneObjects/camera.h"

D3D_FEATURE_LEVEL       Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           Renderer::m_Device{};
ID3D11DeviceContext*    Renderer::m_DeviceContext{};
IDXGISwapChain*         Renderer::m_SwapChain{};
ID3D11RenderTargetView* Renderer::m_RenderTargetView{};
ID3D11DepthStencilView* Renderer::m_DepthStencilView{};

ID3D11Buffer*			Renderer::m_WorldBuffer{};
ID3D11Buffer*			Renderer::m_ViewBuffer{};
ID3D11Buffer*			Renderer::m_ProjectionBuffer{};
ID3D11Buffer*			Renderer::m_MaterialBuffer{};
ID3D11Buffer*			Renderer::m_LightBuffer{};
ID3D11Buffer*           Renderer::m_CameraBuffer{};
ID3D11Buffer*           Renderer::m_ParameterBuffer{};
ID3D11Buffer*           Renderer::m_ReflectionBuffer{};

ID3D11DepthStencilState* Renderer::m_DepthStateEnable{};
ID3D11DepthStencilState* Renderer::m_DepthStateDisable{};
ID3D11DepthStencilState* Renderer::m_StencilWrite{};
ID3D11DepthStencilState* Renderer::m_StencilRead{};

ID3D11RasterizerState*   Renderer::m_RasterStateSolid{};
ID3D11RasterizerState*   Renderer::m_RasterStateWireframe{};
ID3D11RasterizerState*   Renderer::m_RasterStateCallBack{};
ID3D11RasterizerState*   Renderer::m_RasterStateCallNone{};

ID3D11BlendState*		Renderer::m_BlendState{};
ID3D11BlendState*       Renderer::m_BlendStateFalse{};
ID3D11BlendState*		Renderer::m_BlendStateATC{};
ID3D11BlendState*       Renderer::m_BlendStateAdd{};
ID3D11BlendState*       Renderer::m_BlendStateMask{};

//レンダリングテクスチャ関連
ID3D11RenderTargetView*   Renderer::m_PERenderTargetView[]{};
ID3D11DepthStencilView*   Renderer::m_PEDepthStencilView[]{};
ID3D11ShaderResourceView* Renderer::m_PEShaderResourceView[]{};
ID3D11Texture2D*          Renderer::m_PERenderTargetTexture[]{};

//シャドウ
ID3D11DepthStencilView*   Renderer::m_DepthShadowStencilView{};
ID3D11ShaderResourceView* Renderer::m_DepthShadowShaderResourceView{};

//反射用レンダリングテクスチャ
ID3D11RenderTargetView*   Renderer::m_RERenderTargetView{};
ID3D11DepthStencilView*   Renderer::m_REDepthStencilView{};
ID3D11ShaderResourceView* Renderer::m_REShaderResourceView{};
ID3D11Texture2D*          Renderer::m_RERenderTargetTexture{};
XMFLOAT4X4                Renderer::m_ReflectionViewProjFloat4x4{};


void Renderer::Init()
{
	HRESULT hr = S_OK;



	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;
	
	hr = D3D11CreateDeviceAndSwapChain( NULL,
										D3D_DRIVER_TYPE_HARDWARE,
										NULL,
										0,
										NULL,
										0,
										D3D11_SDK_VERSION,
										&swapChainDesc,
										&m_SwapChain,
										&m_Device,
										&m_FeatureLevel,
										&m_DeviceContext );
     



	// レンダーターゲットビュー作成（ポリゴンを描く）
	ID3D11Texture2D* renderTarget{};
	m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&renderTarget );
	m_Device->CreateRenderTargetView( renderTarget, NULL, &m_RenderTargetView );
	renderTarget->Release();



	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	depthStencile->Release();


	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);





	// ビューポート設定
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)SCREEN_WIDTH;
	viewport.Height = (FLOAT)SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports( 1, &viewport );



	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	m_Device->CreateRasterizerState(&rasterizerDesc, &m_RasterStateSolid);

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	m_Device->CreateRasterizerState(&rasterizerDesc, &m_RasterStateWireframe);

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	m_Device->CreateRasterizerState(&rasterizerDesc, &m_RasterStateCallBack);

	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	m_Device->CreateRasterizerState(&rasterizerDesc, &m_RasterStateCallNone);


	m_DeviceContext->RSSetState(m_RasterStateSolid);



	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Device->CreateBlendState( &blendDesc, &m_BlendState );

	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	m_Device->CreateBlendState(&blendDesc, &m_BlendStateFalse);
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_Device->CreateBlendState(&blendDesc, &m_BlendStateAdd);
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0;
	m_Device->CreateBlendState(&blendDesc, &m_BlendStateMask);
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = TRUE;
	m_Device->CreateBlendState( &blendDesc, &m_BlendStateATC );

	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff );





	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthStateEnable );//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthStateDisable );//深度無効ステート

	m_DeviceContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );

	//ステンシル設定
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = TRUE;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR; //+1
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR; //-1
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_StencilWrite);

	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_StencilRead);



	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState{};
	m_Device->CreateSamplerState( &samplerDesc, &samplerState );

	m_DeviceContext->PSSetSamplers( 0, 1, &samplerState );



	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_WorldBuffer );
	m_DeviceContext->VSSetConstantBuffers( 0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_ViewBuffer );
	m_DeviceContext->VSSetConstantBuffers( 1, 1, &m_ViewBuffer );

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_ProjectionBuffer );
	m_DeviceContext->VSSetConstantBuffers( 2, 1, &m_ProjectionBuffer );


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_MaterialBuffer );
	m_DeviceContext->VSSetConstantBuffers( 3, 1, &m_MaterialBuffer );
	m_DeviceContext->PSSetConstantBuffers( 3, 1, &m_MaterialBuffer );


	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_LightBuffer );
	m_DeviceContext->VSSetConstantBuffers( 4, 1, &m_LightBuffer );
	m_DeviceContext->PSSetConstantBuffers( 4, 1, &m_LightBuffer );

	//カメラ
	bufferDesc.ByteWidth = sizeof(XMFLOAT4);
	m_Device->CreateBuffer(&bufferDesc, NULL, &m_CameraBuffer);
	m_DeviceContext->PSSetConstantBuffers(5, 1, &m_CameraBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ParameterBuffer);
	m_DeviceContext->PSSetConstantBuffers(6, 1, &m_ParameterBuffer);

	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ReflectionBuffer);
	m_DeviceContext->PSSetConstantBuffers(7, 1, &m_ReflectionBuffer);

	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = XMFLOAT4(1.0f, -1.0f, 0.5f, 0.0f);
	light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
	light.Position = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	light.PointLightParam = XMFLOAT4(10.0f, 0.0f, 0.0f, 0.0f); //ｘ＝距離
	light.Angle = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); //コーン範囲角度

	light.SkyColor= XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);     //空の色
	light.GroundColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);  //地面の色
	light.GroundNormal = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f); //地面の法線は上向き
	SetLight(light);



	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//レンダリングテクスチャ1を作成する
	{
		//テクスチャ作成
		D3D11_TEXTURE2D_DESC td;      //テクスチャ作成用デスクリプタ構造体変数
		ZeroMemory(&td, sizeof(td));  //構造体を0初期化

		td.Width = swapChainDesc.BufferDesc.Width; //構造体sdはInit関数の最初で作られており
		td.Height = swapChainDesc.BufferDesc.Height;//バックバッファの情報が格納されている

		td.MipLevels = 1;//ミップマップの数 0は限界まで作る
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //テクスチャのピクセルフォーマット
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//使用法のフラグを設定
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&td, NULL, &m_PERenderTargetTexture[0]);


		//レンダリングターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(m_PERenderTargetTexture[0], &rtvd, &m_PERenderTargetView[0]);

		// デプスステンシルビュー作成
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = textureDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Flags = 0;
		m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_PEDepthStencilView[0]);
		depthStencile->Release();
		m_DeviceContext->OMSetRenderTargets(1, &m_PERenderTargetView[0], m_PEDepthStencilView[0]);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(m_PERenderTargetTexture[0], &srvd, &m_PEShaderResourceView[0]);
	}

	//レンダリングテクスチャ2を作成する
	{
		//テクスチャ作成
		D3D11_TEXTURE2D_DESC td;      //テクスチャ作成用デスクリプタ構造体変数
		ZeroMemory(&td, sizeof(td));  //構造体を0初期化

		td.Width = swapChainDesc.BufferDesc.Width; //構造体sdはInit関数の最初で作られており
		td.Height = swapChainDesc.BufferDesc.Height;//バックバッファの情報が格納されている

		td.MipLevels = 1;//ミップマップの数 0は限界まで作る
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //テクスチャのピクセルフォーマット
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//使用法のフラグを設定
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//構造体の設定に従ってテクスチャ領域を作成
		m_Device->CreateTexture2D(&td, NULL, &m_PERenderTargetTexture[1]);


		//レンダリングターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(m_PERenderTargetTexture[1], &rtvd, &m_PERenderTargetView[1]);

		// デプスステンシルビュー作成
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = textureDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Flags = 0;
		m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_PEDepthStencilView[1]);
		depthStencile->Release();
		m_DeviceContext->OMSetRenderTargets(1, &m_PERenderTargetView[1], m_PEDepthStencilView[1]);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(m_PERenderTargetTexture[1], &srvd, &m_PEShaderResourceView[1]);
	}

	//Depthバッファ作成
	{   
		ID3D11Texture2D* ppTexture = NULL;
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));

		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height;

		td.MipLevels = 1;
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R32_TYPELESS;
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		m_Device->CreateTexture2D(&td, NULL, &ppTexture);

		D3D11_DEPTH_STENCIL_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_D32_FLOAT;
		rtvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		m_Device->CreateDepthStencilView(ppTexture, &rtvd, &m_DepthShadowStencilView);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));

		srvd.Format = DXGI_FORMAT_R32_FLOAT;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;

		m_Device->CreateShaderResourceView(ppTexture, &srvd, &m_DepthShadowShaderResourceView);
	}

	//反射用レンダリングテクスチャ作成
	{
		D3D11_TEXTURE2D_DESC td{};
		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		m_Device->CreateTexture2D(&td, nullptr, &m_RERenderTargetTexture);

		//レンダリングターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd{};
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(m_RERenderTargetTexture, &rtvd, &m_RERenderTargetView);

		// デプスステンシルビュー作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = 0;
		m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_REDepthStencilView);
		depthStencile->Release();
		m_DeviceContext->OMSetRenderTargets(1, &m_RERenderTargetView, m_REDepthStencilView);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(m_RERenderTargetTexture, &srvd, &m_REShaderResourceView);
	}
	
}



void Renderer::Uninit()
{

	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();
	m_CameraBuffer->Release();
	m_ParameterBuffer->Release();
	m_ReflectionBuffer->Release();

	m_DepthStateEnable->Release();
	m_DepthStateDisable->Release();
	m_StencilWrite->Release();
	m_StencilRead->Release();

	m_DepthShadowStencilView->Release();
	m_DepthShadowShaderResourceView->Release();

	m_RasterStateSolid->Release();
	m_RasterStateWireframe->Release();
	m_RasterStateCallBack->Release();
	m_RasterStateCallNone->Release();

	m_BlendState->Release();
	m_BlendStateATC->Release();
	m_BlendStateFalse->Release();
	m_BlendStateAdd->Release();

	m_PERenderTargetView[0]->Release();
	m_PEDepthStencilView[0]->Release();
	m_PEShaderResourceView[0]->Release();
	m_PERenderTargetTexture[0]->Release();

	m_PERenderTargetView[1]->Release();
	m_PEDepthStencilView[1]->Release();
	m_PEShaderResourceView[1]->Release();
	m_PERenderTargetTexture[1]->Release();

	m_RERenderTargetView->Release();
	m_REDepthStencilView->Release();
	m_REShaderResourceView->Release();
	m_RERenderTargetTexture->Release();

	m_DeviceContext->ClearState();
	m_RenderTargetView->Release();
	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->Release();
}

void Renderer::Begin()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
}

void Renderer::BeginPE(int n)
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_DeviceContext->ClearRenderTargetView(m_PERenderTargetView[n], clearColor);
	m_DeviceContext->ClearDepthStencilView(m_PEDepthStencilView[n], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	m_DeviceContext->OMSetRenderTargets(1, &m_PERenderTargetView[n], m_PEDepthStencilView[n]);
}

void Renderer::End()
{
	m_SwapChain->Present( 1, 0 );
}

void Renderer::BeginDepth()
{
	m_DeviceContext->OMSetRenderTargets(0, NULL, m_DepthShadowStencilView);
	m_DeviceContext->ClearDepthStencilView(m_DepthShadowStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginReflection()
{
	FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_DeviceContext->ClearRenderTargetView(m_RERenderTargetView, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_REDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	m_DeviceContext->OMSetRenderTargets(1, &m_RERenderTargetView, m_REDepthStencilView);
}

// 反射ビュー更新
void Renderer::UpdateReflectionViewProj(Camera* camera, const Vector3& planeNormal, const Vector3& planePoint)
{
	if (!camera)
	{
		return;
	}

	Vector3 camPos = camera->GetPosition();
	Vector3 camDir = camera->GetDirection();
	Vector3 camUp = camera->GetUp();

	float dist = planeNormal.dot(camPos - planePoint);
	Vector3 reflPos = camPos - 2.0f * dist * planeNormal;
	reflPos += planeNormal * 0.05f;

	Vector3 reflDir = camDir - 2.0f * planeNormal.dot(camDir) * planeNormal;
	if (reflDir.lengthSq() < 0.000001f)
	{
		// 法線と平行 → 代替方向
		reflDir = Vector3(0, 0, 1);
	}

	reflDir.normalize();

	Vector3 reflUp = Vector3(0, 1, 0);

	//View
	XMMATRIX view = XMMatrixLookAtLH
	(
		XMVectorSet(reflPos.x, reflPos.y, reflPos.z, 1.0f),
		XMVectorSet(reflPos.x + reflDir.x, reflPos.y + reflDir.y, reflPos.z + reflDir.z, 1.0f),
		XMVectorSet(reflUp.x, reflUp.y, reflUp.z, 0.0f)
	);
	Renderer::SetViewMatrix(view);

	//Projection
	float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	XMMATRIX proj = XMMatrixPerspectiveFovLH(camera->GetFov(), aspect, 0.3f, 1000.0f);
	Renderer::SetProjectionMatrix(proj);

	//ReflectionViewProj
	XMMATRIX viewProj = view * proj;
	XMMATRIX viewProjT = XMMatrixTranspose(viewProj);
	XMStoreFloat4x4(&m_ReflectionViewProjFloat4x4, viewProjT);
	SetReflection(m_ReflectionViewProjFloat4x4);
}

void Renderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );
	else
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateDisable, NULL );

}

void Renderer::SetBlendEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendStateFalse, blendFactor, 0xffffffff);
}

void Renderer::SetATCEnable( bool Enable )
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

}

void Renderer::SetAddBlendEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateAdd, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);
}

void Renderer::SetRasterSolid(bool Enable)
{
	if (Enable)
		m_DeviceContext->RSSetState(m_RasterStateSolid);
	else
		m_DeviceContext->RSSetState(m_RasterStateWireframe);
}

void Renderer::SetStencilShadowWrite()
{
	//カラーバッファ書き込み無効
	float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };
	m_DeviceContext->OMSetBlendState(m_BlendStateMask, blendFactor, 0xffffffff);

	//カリングOFF(両面描画)
	m_DeviceContext->RSSetState(m_RasterStateCallNone);

	//ステンシル書き込み
	m_DeviceContext->OMSetDepthStencilState(m_StencilWrite, NULL);
}

void Renderer::SetStencilShadowRead()
{
	//カラーバッファ書き込み有効
	float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };
	m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

	//カリングON(表面描画)
	m_DeviceContext->RSSetState(m_RasterStateCallBack);

	//ステンシル書き込み
	m_DeviceContext->OMSetDepthStencilState(m_StencilRead, NULL);
}

void Renderer::SetStencilShadowNone()
{
	//ステンシル無効
	m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
}

void Renderer::ClearDepth()
{
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::SetWorldViewProjection2D()
{
	SetWorldMatrix(XMMatrixIdentity());
	SetViewMatrix(XMMatrixIdentity());

	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);
}


void Renderer::SetWorldMatrix(XMMATRIX WorldMatrix)
{
	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, XMMatrixTranspose(WorldMatrix));
	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &worldf, 0, 0);
}

void Renderer::SetViewMatrix(XMMATRIX ViewMatrix)
{
	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, XMMatrixTranspose(ViewMatrix));
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &viewf, 0, 0);
}

void Renderer::SetProjectionMatrix(XMMATRIX ProjectionMatrix)
{
	XMFLOAT4X4 projectionf;
	XMStoreFloat4x4(&projectionf, XMMatrixTranspose(ProjectionMatrix));
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projectionf, 0, 0);

}

void Renderer::SetMaterial( MATERIAL Material )
{
	m_DeviceContext->UpdateSubresource( m_MaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void Renderer::SetLight( LIGHT Light )
{
	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetCameraPosition(XMFLOAT3 CameraPosition)
{
	XMFLOAT4	temp = XMFLOAT4(CameraPosition.x, CameraPosition.y, CameraPosition.z, 0.0f);
	GetDeviceContext()->UpdateSubresource(m_CameraBuffer, 0, NULL, &temp, 0, 0);
}

void Renderer::SetParameter(XMFLOAT4 Parameter)
{
	GetDeviceContext()->UpdateSubresource(m_ParameterBuffer, 0, NULL, &Parameter, 0, 0);
}

void Renderer::SetReflection(XMFLOAT4X4 Reflection)
{
	GetDeviceContext()->UpdateSubresource(m_ReflectionBuffer, 0, NULL, &Reflection, 0, 0);
}

void Renderer::SetSpriteUV(float ox, float oy, float sx, float sy)
{
	SetParameter(XMFLOAT4(ox, oy, sx, sy));
}

XMFLOAT3 Renderer::ScreenToWorld(int mouseX, int mouseY, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
	// スクリーン座標 -> NDC（-1 ~ +1）
	float ndcX = (2.0f * mouseX) / SCREEN_WIDTH - 1.0f;
	float ndcY = ((2.0f * mouseY) / SCREEN_HEIGHT - 1.0f) * -1.0f; // Y反転

	// NDC → クリップ空間の点（Z = 1.0f = FarPlane）
	XMVECTOR clipSpacePos = XMVectorSet(ndcX, ndcY, 1.0f, 1.0f);

	// ビュー×プロジェクションの逆行列を計算
	XMMATRIX invViewProj = XMMatrixInverse(nullptr, viewMatrix * projectionMatrix);

	// 逆行列でワールド空間に変換
	XMVECTOR worldPos = XMVector3TransformCoord(clipSpacePos, invViewProj);

	// XMVECTOR → XMFLOAT3 に変換して返す
	XMFLOAT3 result;
	XMStoreFloat3(&result, worldPos);
	return result;
}

void Renderer::PrintGPUName(ID3D11Device* device)
{
	IDXGIDevice* dxgiDevice = nullptr;
	device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* adapter = nullptr;
	dxgiDevice->GetAdapter(&adapter);

	DXGI_ADAPTER_DESC desc;
	adapter->GetDesc(&desc);

	std::wstring gpuName = L"使用中のGPU: ";
	gpuName += desc.Description;
	OutputDebugStringW((gpuName + L"\n").c_str());

	adapter->Release();
	dxgiDevice->Release();
}

void Renderer::CreateVertexShader( ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName )
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader( ID3D11PixelShader** PixelShader, const char* FileName )
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}


