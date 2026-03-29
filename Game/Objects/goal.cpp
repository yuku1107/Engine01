/*==============================================================================

	ƒS[ƒ‹[goal.cpp]
															 Author:Šs@“N‰F
															 Date  :2025/09/13
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Components/shader.h"
#include "Engine/Components/shape.h"
#include "goal.h"


void Goal::Init()
{
    m_Model = new AnimationModel;
    m_Model->Load("Assets\\model\\goal.fbx");

    m_MaterialColor = { 1.0f,1.0f,0.0f,1.0f };

    AddComponent<Capsule>()->Set(this);

    m_ShaderName = "pointLightingBlinnPhong";
    AddComponent<Shader>()->Set(this);

    m_Tag = "GOAL";
}

void Goal::UninitSelf()
{
    m_Model->Uninit();
    delete m_Model;

}

void Goal::UpdateSelf()
{
    if (Manager::GetCreaterMode())
    {
        m_Rotation.y = 0.0f;
    }
    else
    {
        m_Rotation.y += 0.01f;
    }

}

void Goal::DrawSelf()
{
    //ƒ}ƒgƒŠƒNƒXÝ’è
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    world = scale * rotation * trancelation;

    Renderer::SetWorldMatrix(world);

    m_Model->Draw(m_MaterialColor);
}