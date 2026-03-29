/*==============================================================================

    ƒAƒ“ƒJ[[anchor.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/07/14
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Components/rigidbody.h"
#include "Engine/Serialization/serializer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "wireComponent.h"
#include "anchor.h"
#include "player.h"

void Anchor::Init()
{
    m_Position = m_StartPosition;

    m_Model = new AnimationModel;
    m_Model->Load("Assets\\model\\Collision_sphere.fbx");

    m_Scale = Vector3(0.5f, 0.5f, 0.5f);

    m_Tag = "ANCHOR";

    m_ShaderName = "unlitTexture";
    AddComponent<Sphere>()->Set(this);
    AddComponent<Shader>()->Set(this);

    auto* rb = AddComponent<Rigidbody>();
    rb->Set(this);
    rb->SetMass(1.0f);
    rb->SetResistanceMap("Air", Vector3(0.5f, 0.5f, 0.5f));
    rb->SetMovePowerMap("Air", 5.0f);
    rb->SetMovementMode("Air");
    rb->SetUseGravity(false);

    AddComponent<WireComponent>();
}

void Anchor::UninitSelf()
{
    m_Model->Uninit();
    delete m_Model;
}

void Anchor::UpdateSelf()
{
    Player* player = Manager::GetScene()->GetGameObject<Player>();

    if (player)
    {
        auto* wireComp = GetComponent<WireComponent>();

        if ((player->GetPosition() - m_Position).lengthSq() <= (player->GetEffDistance() * player->GetEffDistance()))
        {
            if (wireComp->IsTargeted())
            {
                m_MaterialColor = XMFLOAT4(1.0f,0.0f,0.0f,1.0f);
            }
            else
            {
                m_MaterialColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
            }
        }
        else
        {
            wireComp->Clear();
            m_MaterialColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
        }
    }

    bool nowCreater = Manager::GetCreaterMode();

    if (nowCreater && !m_WasCreaterMode)
    {
        m_Position = m_StartPosition;
    }

    if(!nowCreater)
    {
        auto* rb = GetComponent<Rigidbody>();

        Vector3 target = m_GoEnd ? m_EndPosition : m_StartPosition;
        Vector3 toTarget = target - m_Position;

        float distSq = toTarget.lengthSq();

        // “ž’B‚µ‚½‚ç”½“]
        if (distSq < m_StopDistance * m_StopDistance)
        {
            m_GoEnd = !m_GoEnd;
            rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
            return;
        }

        // ³‹K‰»‚µ‚Ä“ü—Í•ûŒü‚É‚·‚é
        Vector3 dir = toTarget.normalized();

        rb->AddVelocity(dir);

        m_Position = rb->GetPositionNext();
    }

    m_WasCreaterMode = nowCreater;
}

void Anchor::DrawSelf()
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

void Anchor::DrawInspector()
{
    float startPosition[4] = { m_Position.x,m_Position.y,m_Position.z,0.0f };
    ImGui::Text("StartPosition:");
    ImGui::InputFloat3("posStart", startPosition);
    Vector3 startPos = { startPosition[0],startPosition[1],startPosition[2] };
    m_StartPosition = startPos;

    float endPosition[4] = { m_EndPosition.x,m_EndPosition.y,m_EndPosition.z,0.0f };
    ImGui::Text("EndPosition:");
    ImGui::InputFloat3("endStart", endPosition);
    Vector3 endPos = { endPosition[0],endPosition[1],endPosition[2] };
    m_EndPosition = endPos;
}

void Anchor::Serialize(Serializer& s)
{
    GameObject::Serialize(s);
     s.WriteVector3("startPosition", m_StartPosition);
	 s.WriteVector3("endPosition", m_EndPosition);
}

void Anchor::Deserialize(Serializer& s)
{
    GameObject::Deserialize(s);
    m_StartPosition = s.ReadVector3("startPosition");
    m_EndPosition = s.ReadVector3("endPosition");
}
