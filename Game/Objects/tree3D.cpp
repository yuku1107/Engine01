/*==============================================================================

    –Ø_3D[tree3D.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/12/11
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Components/shader.h"
#include "Engine/Components/shape.h"
#include "tree3D.h"

void Tree3D::Init()
{
    m_Model = new AnimationModel;
    m_Model->Load("Assets\\model\\tree.fbx");

    m_Tag = "TREE3D";

    m_ShaderName = "pointLightingBlinnPhong";
    AddComponent<Shader>()->Set(this);

    AddComponent<Box>()->Set(this);

}

void Tree3D::UninitSelf()
{
    m_Model->Uninit();
    delete m_Model;
}

void Tree3D::UpdateSelf()
{

}

void Tree3D::DrawSelf()
{


    //ƒ}ƒgƒŠƒNƒXÝ’è
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x + XM_PIDIV2, m_Rotation.y, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    world = scale * rotation * trancelation;

    Renderer::SetWorldMatrix(world);

    m_Model->Draw(m_MaterialColor);
}
