/*==============================================================================

    シャドウ[shadowVolume.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/11/14
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/Mesh/mesh2D.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Components/shader.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "shadowVolume.h"

void ShadowVolume::Init()
{
    m_SaveEnable = false;


    m_Model = AddComponent<AnimationModel>();
    m_Model->Load("Assets\\model\\eyeShadow.fbx");

    m_Mesh2D = new Mesh2D();
    m_Mesh2D->Init();

    //テクスチャ読込
    m_TextureName = "white";
    m_Texture = Texture::Get(m_TextureName);

    m_ShaderName = "unlitTexture";
    AddComponent<Shader>()->Set(this);

    m_Tag = "SHADOWVOLUME";

    m_MaterialColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
}

void ShadowVolume::UninitSelf()
{
	m_Model = nullptr;

    m_Mesh2D->Uninit();
    delete m_Mesh2D;
}

void ShadowVolume::UpdateSelf()
{

}

void ShadowVolume::DrawSelf()
{
    //マトリクス設定
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y + XM_PI, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    world = scale * rotation * trancelation;

    Renderer::SetWorldMatrix(world);

    Renderer::SetStencilShadowWrite();

    m_Model->Draw();

    Renderer::SetStencilShadowRead();

    //テクスチャ設定
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
    
    //影2Dポリゴン描画
    Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 rot = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 scl = Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f);
    m_Mesh2D->Draw(pos,rot,scl, m_MaterialColor);
    
    Renderer::SetStencilShadowNone();
    
    Manager::GetScene()->GetGameObject<Camera>()->Draw();
}