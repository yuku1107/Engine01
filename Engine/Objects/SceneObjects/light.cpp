/*==============================================================================

    光源[Light.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/22
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/shader.h"
#include "Engine/Components/shape.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "player.h"
#include "light.h"

void Light::Init()
{
    m_Model = new AnimationModel;
    m_Model->Load("Assets\\model\\Light.fbx");

    //ライトパラメーター設定
    m_Light.Position = XMFLOAT4(m_Position.x, m_Position.y, m_Position.z, 1.0f);
    m_Light.Diffuse = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
    m_Light.Direction = XMFLOAT4(1.0f, -1.0f, 0.5f, 0.0f);
    m_Light.PointLightParam = XMFLOAT4(100.0f, 0.0f, 0.0f, 0.0f);         
    m_Light.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);   
    m_Light.SkyColor = { 0.5f, 0.7f, 1.0f, 1.0f }; 
    m_Light.GroundColor = { 0.2f, 0.2f, 0.2f, 1.0f }; 
    m_Light.GroundNormal = { 0.0f, 1.0f, 0.0f, 0.0f };
    m_Light.Angle = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); 

    AddComponent<Sphere>()->Set(this);

    m_ShaderName = "unlitTexture";
    AddComponent<Shader>()->Set(this);

    m_Tag = "LIGHT";
}

void Light::UninitSelf()
{
    m_Model->Uninit();
    delete m_Model;
}

void Light::UpdateSelf()
{
    Vector3 targetPos = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 lightPos = Vector3(-8.0f, 10.0f, -8.0f);
    GameObject* targetObject = nullptr;

    if (Manager::GetCreaterMode())
    {
        targetObject = Manager::GetScene()->GetGameObject<Camera>();
    }
    else
    {
        targetObject = Manager::GetScene()->GetGameObject<Player>();
    }

    targetPos = targetObject->GetPosition();
    m_Position = targetPos + lightPos;
    m_Light.Position = XMFLOAT4(m_Position.x, m_Position.y, m_Position.z, 0.0f);

    //ライトカメラのビュー行列
    XMVECTOR lpos = XMVectorSet(m_Light.Position.x, m_Light.Position.y, m_Light.Position.z, 0.0f);
    XMVECTOR lat = XMVectorSet(targetPos.x, targetPos.y, targetPos.z, .0f);
    XMVECTOR lup = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_Light.ViewMatrix = XMMatrixLookAtLH(lpos, lat, lup);
    Renderer::SetViewMatrix(m_Light.ViewMatrix);

    //ライトカメラのプロジェクション行列
    //m_Light.ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
    float shadowRange = 20.0f;
    m_Light.ProjectionMatrix = XMMatrixOrthographicLH(shadowRange, shadowRange, 0.0f, 20.0f);
    Renderer::SetProjectionMatrix(m_Light.ProjectionMatrix);
    
    m_Light.ViewMatrix = XMMatrixTranspose(m_Light.ViewMatrix);
    m_Light.ProjectionMatrix = XMMatrixTranspose(m_Light.ProjectionMatrix);



}

void Light::DrawSelf()
{
    //マトリクス設定
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    world = scale * rotation * trancelation;

    Renderer::SetWorldMatrix(world);

    //m_Model->Draw();
}
