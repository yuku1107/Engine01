/*==============================================================================

    天空[sky.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/11
==============================================================================*/
#include "main.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Components/shader.h"
#include "Engine/Serialization/serializer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "sky.h"

void Sky::Init()
{
    m_Model = AddComponent<AnimationModel>();

    switch (m_Type)
    {
    case 1:
        m_Model->Load("Assets\\model\\sky2.fbx");
        break;
    case 2:
        m_Model->Load("Assets\\model\\sky_D.fbx");
        break;
    default:
        m_Model->Load("Assets\\model\\sky2.fbx");
        break;
    }
    m_TypeNext = m_Type;

    m_Tag = "SKY";

    m_ShaderName = "unlitTexture";
    AddComponent<Shader>()->Set(this);
}

void Sky::UninitSelf()
{
	m_Model = nullptr;
}

void Sky::UpdateSelf()
{
    if (m_TypeNext != m_Type)
    {
        m_Type = m_TypeNext;

        switch (m_Type)
        {
        case 1:
            m_Model->Load("Assets\\model\\sky2.fbx");
            break;
        case 2:
            m_Model->Load("Assets\\model\\sky_D.fbx");
            break;
        }
    }
}

void Sky::DrawSelf()
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

void Sky::DrawInspector()
{
    int skyType = m_Type;
    ImGui::Text("Type:");
    ImGui::InputInt("type", &skyType);
    m_TypeNext = skyType;
}

void Sky::Serialize(Serializer& s)
{
	GameObject::Serialize(s);

	s.WriteInt("Type", m_Type);
}

void Sky::Deserialize(Serializer& s)
{
	GameObject::Deserialize(s);

	m_Type = s.ReadInt("Type");
}
