/*==============================================================================

    ìG[enemy.cpp]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/06/04
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
#include "enemy.h"
#include "enemyStateSearch.h"
#include "enemyStateAttack.h"
#include "enemyStateAttackBullet.h"
#include "enemyStateDead.h"
#include "enemyStateTitle.h"
#include "enemyStateResult.h"
#include "enemyStateReturn.h"
#include "enemyStateCreater.h"
#include "shadowVolume.h"

#include "player.h"

void Enemy::ChangeState(EnemyState* newState)
{
    if (m_CurrentState)
    {
        m_CurrentState->Exit(this);
        delete m_CurrentState;
    }

    m_CurrentState = newState;

    if (m_CurrentState)
    {
        m_CurrentState->Enter(this);
    }
}

void Enemy::Init()
{
    m_Model = AddComponent<AnimationModel>();
    m_Model->Load("Assets\\model\\gost.fbx");

    m_Model->LoadAnimation("Assets\\model\\gost_Run.fbx", "Run");
    m_Model->LoadAnimation("Assets\\model\\gost_Idle.fbx", "Idle");
    m_Model->LoadAnimation("Assets\\model\\gost_Attack.fbx", "Attack");

    m_AnimationName = "Idle";
    m_AnimationNameNext = "Idle";
    m_AnimationBlend = 0.0f;
    m_Frame = 0;

    m_ShaderName = "pointLightingBlinnPhong";

    auto rb = AddComponent<Rigidbody>();
    rb->Set(this);
    rb->SetMass(-1.0f);
    rb->SetUseGravity(false);

    AddComponent<Capsule>()->Set(this);
    //Å´èáî‘
    AddComponent<Shader>()->Set(this);

    m_Tag = "ENEMY";
 
    ChangeState(new EnemyStateSearch());
    m_ShadowVolume = Manager::GetScene()->AddGameObject<ShadowVolume>(4);
    m_ShadowVolume->SetScale(Vector3(9.5f, 0.5f, 9.5f));
    
    AddComponent<WireComponent>();
}

void Enemy::UninitSelf()
{
	m_Model = nullptr;

    if(m_ShadowVolume)
    {   
        m_ShadowVolume->SetDestroy();
    }
   
}

void Enemy::UpdateSelf()
{
    float deltaTime = 1.0f / 120.0f;

    auto* rb = GetComponent<Rigidbody>();
    auto* capsule = GetComponent<Capsule>();
    
    Vector3 direction;

    if (m_Player == nullptr)
    {
        Player* player = Manager::GetScene()->GetGameObject<Player>();

        if (player != nullptr)
        {
            m_Player = player;
        }
    }
    else
    {
        direction = m_Player->GetPosition() - m_Position;
    }

    if (m_CurrentState)
    {
        m_CurrentState->Update(this, deltaTime);
    }

    if (Manager::GetCreaterMode())
    {
        if (!IsStateCreater())
        {
            ChangeState(new EnemyStateCreater);
            m_ShadowVolume->SetPosition(m_Position - Vector3(0.0f, 1.0f, 0.0f) + GetForward() * 0.5f);
        }
        return;
    }

    if (direction.length() <= m_EnableDistance)
    {
        m_DrawEnable = true;
    }
    else
    {
        m_DrawEnable = false;
    }

    if (m_ShadowVolume)
    {
        m_ShadowVolume->SetPosition(m_Position - Vector3(0.0f, 1.0f, 0.0f) + GetForward() * 0.5f);
        m_ShadowVolume->SetRotation(m_Rotation);
        if (IsStateSearch())
        {
            m_ShadowVolume->SetDrawEnable(m_DrawEnable);
        }
        else
        {
            m_ShadowVolume->SetDrawEnable(false);
        }
    }

    if (m_Life <= 0)
    {
        if (!IsStateDead())
        {
            ChangeState(new EnemyStateDead());
        }
        return;
    }

    if (!IsStateAttack())
    {
        m_Model->Update(m_AnimationName.c_str(), m_Frame, m_AnimationNameNext.c_str(), m_Frame, m_AnimationBlend);
    }
    else
    {
        m_IsAttacking = m_Model->Update(m_AnimationName.c_str(), m_Frame, m_AnimationNameNext.c_str(), m_Frame, m_AnimationBlend, false);
    }

    m_Frame ++;
    m_AnimationBlend += 0.02f;
    if (m_AnimationBlend > 1.0f)
    {
        m_AnimationBlend = 1.0f;
    }

    m_Position = rb->GetPositionNext();
}

void Enemy::DrawSelf()
{
    //É}ÉgÉäÉNÉXê›íË
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    world = scale * rotation * trancelation;

    Renderer::SetWorldMatrix(world);

    m_Model->Draw(m_MaterialColor);
}

void Enemy::DrawInspector()
{
    int enemy_type = m_AttackType;
    ImGui::Text("Type:");
    ImGui::InputInt("type", &enemy_type);
    m_AttackType = enemy_type;

    float startPosition[4] = { m_Position.x,m_Position.y,m_Position.z,0.0f };
    ImGui::Text("StartPosition:");
    ImGui::InputFloat3("posStart", startPosition);
    Vector3 startPos = { startPosition[0],startPosition[1],startPosition[2] };
    m_Position = startPos;

    float startRotation[4] = { m_Rotation.x,m_Rotation.y,m_Rotation.z,0.0f };
    ImGui::Text("StartRotation:");
    ImGui::InputFloat3("rotStart", startRotation);
    Vector3 startRot = { startRotation[0],startRotation[1],startRotation[2] };
    m_Rotation = startRot;

    float moveMax = m_MoveMax;
    ImGui::Text("MoveMax:");
    ImGui::InputFloat("Max", &moveMax);
    m_MoveMax = moveMax;
}

void Enemy::AnimationBlend(std::string AnimationName)
{
    if (m_AnimationNameNext != AnimationName)
    {
        m_AnimationName = m_AnimationNameNext;
        m_AnimationNameNext = AnimationName;
        m_AnimationBlend = 0.0f;
    }
}

bool Enemy::IsStateSearch() const
{
    return dynamic_cast<EnemyStateSearch*>(m_CurrentState) != nullptr;
}

bool Enemy::IsStateReturn() const
{
    return dynamic_cast<EnemyStateReturn*>(m_CurrentState) != nullptr;
}

bool Enemy::IsStateAttack() const
{
    return dynamic_cast<EnemyStateAttack*>(m_CurrentState) != nullptr || dynamic_cast<EnemyStateAttackBullet*>(m_CurrentState) != nullptr;
}

bool Enemy::IsStateDead() const
{
    return dynamic_cast<EnemyStateDead*>(m_CurrentState) != nullptr;
}

bool Enemy::IsStateCreater() const
{
    return dynamic_cast<EnemyStateCreater*>(m_CurrentState) != nullptr;
}

void Enemy::CanBeWire(bool can)
{
    auto* wireComp = GetComponent<WireComponent>();
    if (wireComp)
    {
        wireComp->SetWireable(can);
	}
}

void Enemy::Serialize(Serializer& s)
{
	GameObject::Serialize(s);

    s.WriteVector3("startPosition", m_StartPosition);
    s.WriteVector3("startRotation", m_StartRotation);
    s.WriteFloat("moveMax", m_MoveMax);
	s.WriteInt("attackType", m_AttackType);
}

void Enemy::Deserialize(Serializer& s)
{
	GameObject::Deserialize(s);

    m_StartPosition = s.ReadVector3("startPosition");
    m_StartRotation = s.ReadVector3("startRotation");
	m_MoveMax = s.ReadFloat("moveMax");
	m_AttackType = s.ReadInt("attackType");
}

