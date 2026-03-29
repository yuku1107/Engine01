/*==============================================================================

	ínñ _à⁄ìÆ[groundMove.cpp]
															 Author:äsÅ@ìNâF
															 Date  :2025/12/18
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Components/shader.h"
#include "Engine/Components/shape.h"
#include "Engine/Serialization/serializer.h"
#include "ThirdParty/ImGui/imgui.h"

#include "groundMove.h"


void GroundMove::Init()
{
    m_Position = m_StartPosition;

    m_Frame = 0.0f;
    m_Model = AddComponent<AnimationModel>();
    m_Model->Load("Assets\\model\\Cube_black.fbx");

    m_ShaderName = "depthShadow";

    AddComponent<Box>()->Set(this, Shape::Block);
    AddComponent<Shader>()->Set(this);

    m_Tag = "GROUNDMOVE";
}

void GroundMove::Init(Vector3 start, Vector3 End)
{
    m_Frame = 0.0f;

    m_StartPosition = start;
    m_EndPosition = End;

    m_Model = AddComponent<AnimationModel>();
    m_Model->Load("asset\\model\\Cube_digital.fbx");

    m_ShaderName = "depthShadow";

    AddComponent<Box>()->Set(this, Shape::Block);
    AddComponent<Shader>()->Set(this);

    m_Tag = "GROUNDMove";
}

void GroundMove::UninitSelf()
{
	m_Model = nullptr;
}

void GroundMove::UpdateSelf()
{
    bool nowCreater = Manager::GetCreaterMode();

    if (nowCreater && !m_WasCreaterMode)
    {
        m_Position = m_StartPosition;
    }

    if (!nowCreater)
    {
        m_Position = Lerp(m_StartPosition, m_EndPosition, m_Frame);

        if (m_GoEnd)
        {
            m_Frame += 0.1f * m_Speed;

            if (m_Frame >= 1.0f)
            {
                m_GoEnd = false;
            }
        }
        else
        {
            m_Frame -= 0.1f * m_Speed;

            if (m_Frame <= 0.0f)
            {
                m_GoEnd = true;
            }
        }
    }

    m_WasCreaterMode = nowCreater;
}

void GroundMove::DrawSelf()
{
    //É}ÉgÉäÉNÉXê›íË
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    world = scale * rotation * trancelation;

    Renderer::SetWorldMatrix(world);

    m_Model->Draw();
}

void GroundMove::DrawInspector()
{
    float start[4] = { m_Position.x, m_Position.y, m_Position.z, 0.0f };
    ImGui::Text("Start:");
    ImGui::InputFloat3("posStart", start);
    Vector3 startV3 = { start[0],start[1],start[2] };
    m_Position = startV3;

    float end[4] = { m_EndPosition.x, m_EndPosition.y, m_EndPosition.z, 0.0f };
    ImGui::Text("End:");
    ImGui::InputFloat3("posEnd", end);
    Vector3 endV3 = { end[0],end[1],end[2] };
    m_EndPosition = endV3;

    float speed = m_Speed;
    ImGui::Text("Speed:");
    ImGui::InputFloat("Speed", &speed);
    m_Speed = speed;
}

void GroundMove::Serialize(Serializer& s)
{
    GameObject::Serialize(s);

    s.WriteVector3("startPosition", m_StartPosition);
    s.WriteVector3("endPosition", m_EndPosition);
    s.WriteFloat("speed", m_Speed);
}

void GroundMove::Deserialize(Serializer& s)
{
	GameObject::Deserialize(s);

	m_StartPosition = s.ReadVector3("startPosition");
	m_EndPosition = s.ReadVector3("endPosition");
	m_Speed = s.ReadFloat("speed");
}
