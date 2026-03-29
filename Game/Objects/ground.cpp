/*==============================================================================

    地面[ground.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/09/11
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Components/shader.h"
#include "Engine/Components/shape.h"
#include "ground.h"


void Ground::Init()
{
    m_Model = AddComponent<AnimationModel>();
    m_Model->Load("Assets\\model\\Cube_black.fbx");

    m_ShaderName = "depthShadow";

    AddComponent<Box>()->Set(this,Shape::Block);
    AddComponent<Shader>()->Set(this);

    m_Tag = "GROUND";
}

void Ground::UninitSelf()
{
	m_Model = nullptr;
}

void Ground::UpdateSelf()
{
    
}

void Ground::DrawSelf()
{
    //マトリクス設定
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    world = scale * rotation * trancelation;

    Renderer::SetWorldMatrix(world);

    m_Model->Draw();
}