/*==============================================================================

    ターゲットマーク[TargetMark.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/09/08
==============================================================================*/
#include "main.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Components/shader.h"
#include "Engine/Math/easing.h"
#include "targetMark.h"

void TargetMark::Init()
{
    m_Model = AddComponent<AnimationModel>();
    m_Model->Load("Assets\\model\\Mark.fbx");
    
    m_Scale = { 1.0f,1.0f,1.0f };

    m_Interpolation = 0.0f;
    m_Count = 0.05f;

    m_Tag = "TARGETMARK";

    m_ShaderName = "unlitTexture";
    AddComponent<Shader>()->Set(this);
}

void TargetMark::UninitSelf()
{
	m_Model = nullptr;
}

void TargetMark::UpdateSelf()
{
    if (m_Target)
    {
        m_Position = m_Target->GetPosition() + Vector3(0.0f, m_Target->GetScale().y * 1.5f, 0.0f);
        float f = easeInOutSine(m_Interpolation);

        m_Position.y += f * 0.2f;

        m_Interpolation += m_Count;
    }
}

void TargetMark::DrawSelf()
{
    if (m_Target)
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
}