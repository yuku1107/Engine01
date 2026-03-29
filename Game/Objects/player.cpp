/*==============================================================================

    プレイヤー[player.cpp]
                                                             Author:郭　哲宇
                                                             Date  :
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Collision/collision.h"
#include "Engine/Components/audio.h"
#include "Engine/Components/rigidbody.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Serialization/serializer.h"
#include "ThirdParty/ImGui/imgui.h"

#include "wireComponent.h"

#include "player.h"
#include "playerStateAir.h"
#include "playerStateGround.h"
#include "playerStateCreater.h"
#include "playerStateDead.h"
#include "playerStateInvincible.h"
#include "playerStateClear.h"
#include "playerStateWire.h"
#include "playerStateAssassin.h"

#include "wire.h"
#include "enemy.h"
#include "bullet.h"
#include "targetMark.h"
#include "Game/Input/gameInput.h"

void Player::ChangeState(PlayerState* newState)
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

void Player::Init()
{
    m_Position = m_StartPosition;
    m_Rotation = m_StartRotation;
    m_ReSpawnPosition = m_Position;

    m_Life = m_MaxLife;

    m_Model = AddComponent<AnimationModel>();
    m_Model->Load("Assets\\model\\hunter.fbx");
    m_Model->LoadAnimation("Assets\\model\\hunter_Run.fbx", "Run");
    m_Model->LoadAnimation("Assets\\model\\hunter_Idle.fbx", "Idle");
    m_Model->LoadAnimation("Assets\\model\\hunter_JumpUp.fbx", "JumpUp");
    m_Model->LoadAnimation("Assets\\model\\hunter_Sleep.fbx", "Sleep");
    m_Model->LoadAnimation("Assets\\model\\hunter_Clear.fbx", "Clear");
    m_Model->LoadAnimation("Assets\\model\\hunter_Byebye.fbx", "Byebye");

    m_AnimationName = "Idle";
    m_AnimationNameNext = "Idle";
    m_AnimationBlend = 0.0f;
    m_Frame = 0;

	m_SE = AddComponent<Audio>();
    m_SE->Load("Assets\\audio\\se_hit_004.wav");

    m_SEjump = AddComponent<Audio>();
    m_SEjump->Load("Assets\\audio\\jump.wav");

    m_IsGrounded = false;

    m_ShaderName = "pointLightingBlinnPhong";

    m_Wire = Manager::GetScene()->AddGameObject<Wire>(6);

    m_BulletNum = 3;

    auto* rb = AddComponent<Rigidbody>();

    rb->Set(this);
    //抵抗力
    rb->SetResistanceMap("Air", Vector3(0.5f, 0.2f, 0.5f));
    rb->SetResistanceMap("Ground", Vector3(0.7f, 0.2f, 0.7f));
    rb->SetResistanceMap("Wire", Vector3(0.01f, 0.1f, 0.01f));
    rb->SetCurrentResistance("Air");
    //移動力
    rb->SetMovePowerMap("Air", 10.0f);
    rb->SetMovePowerMap("Ground", 20.0f);
    rb->SetMovePowerMap("Wire", 0.3f);
    rb->SetCurrentMovePower("Air");

    AddComponent<Capsule>()->Set(this);
    //↓順番
    AddComponent<Shader>()->Set(this);

    ChangeState(new PlayerStateAir());
    
    m_Tag = "PLAYER";
}

void Player::UninitSelf()
{
    m_SE = nullptr;

    m_SEjump = nullptr;

    m_Model = nullptr;
}

void Player::UpdateSelf()
{
    float deltaTime = 1.0f / 60.0f;

    if (m_CurrentState)
    {
        m_CurrentState->Update(this, deltaTime);
    }

    if (Manager::GetCreaterMode())
    {
        if (!IsStateCreater())
        {
            ChangeState(new PlayerStateCreater);
        }

        return;
    }

    if (!m_DrawEnable)
    {
        if (dynamic_cast<PlayerStateDead*>(m_CurrentState) == nullptr)
        {
            ChangeState(new PlayerStateDead());
        }
    }
    else
    {
        Heal();
    }

    ResolveCollisions();
    
    if (m_IsAnimationLoop)
    {
        m_Model->Update(m_AnimationName.c_str(), m_Frame, m_AnimationNameNext.c_str(), m_Frame, m_AnimationBlend);
    }
    else
    {
        if (m_IsJumping)
        {
            m_IsJumpUp= m_Model->Update(m_AnimationName.c_str(), m_Frame, m_AnimationNameNext.c_str(), m_Frame, m_AnimationBlend, false);
        }
    }

    m_Frame++;
    m_AnimationBlend += 0.1f;
    if (m_AnimationBlend > 1.0f)
    {
        m_AnimationBlend = 1.0f;
    }
    
    if (m_Position.y <= -5.0f)
    {
        m_Life--;
        SetReturn();
    }
}

void Player::DrawSelf()
{
    XMMATRIX parentMatrix;

    //親描画
    {
        //マトリクス設定
        XMMATRIX world, scale, rotation, trancelation;

        scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
        rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
        trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

        world = scale * rotation * trancelation;
        parentMatrix= rotation * trancelation;

        Renderer::SetWorldMatrix(world);

        m_Model->Draw(m_MaterialColor);
    }
}

void Player::DrawInspector()
{
    auto rb = GetComponent<Rigidbody>();

    float Object_vel[4] = { rb->GetVelocity().x,rb->GetVelocity().y,rb->GetVelocity().z,0.0f };
    ImGui::Text("Velocity: %.2f %.2f %.2f ", Object_vel[0], Object_vel[1], Object_vel[2]);

    if (GetIsGrounded())
    {
        ImGui::Text("IsGound : true");
    }
    else
    {
        ImGui::Text("IsGound : false");
    }

    if (GetIsJump())
    {
        ImGui::Text("IsJump : true");
    }
    else
    {
        ImGui::Text("IsJump : false");
    }

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
}

//当たり判定＆位置補正
void Player::ResolveCollisions()
{
    auto* capsule = GetComponent<Capsule>();
    auto* rb = GetComponent<Rigidbody>();

    //当たり判定の更新
    capsule->Update(rb->GetPositionNext() + Vector3(0.0f, 0.06f, 0.0f), m_Scale, m_Rotation);

    Vector3 totalNormal = Vector3(0.0f, 0.0f, 0.0f);
    float totalPenetration = 0.0f;
    int hitCount = 0;

    auto shapes = Manager::GetCollision()->ShapeCollision(*capsule);

    if (!shapes.empty())
    {
        for (auto shape : shapes)
        {
            if (shape->GetpOwner()->GetTag() == "GOAL")
            {
                if (!IsStateClear())
                {
                    ChangeState(new PlayerStateClear());
                }
            }

            switch (shape->GetCollisionResponse())
            {
            case Shape::Block:

                Vector3 normal = Vector3(0.0f, 0.0f, 0.0f);
                float penetration = 0.0f;

                if (capsule->Intersect(*shape, &normal, &penetration))
                {
                    totalNormal += normal;
                    totalPenetration += penetration;
                    hitCount++;
                    if (normal.y > 0.5f)
                    {
                        m_IsGrounded = true;

                        //足元のブロック記録
                        if (shape->GetpOwner()->GetTag() == "GROUND")
                        {
                            m_ReSpawnPosition = shape->GetPosition() + Vector3(0.0f, 2.0f, 0.0f);
                        }
                    }
                }
                break;
            }
        }

        m_IsGrounded = Manager::GetCollision()->MultiRaycast(*capsule, Vector3(0.0f, -1.0f, 0.0f), m_ground, 0.1f);
    }
    else
    {
        m_IsGrounded = Manager::GetCollision()->MultiRaycast(*capsule, Vector3(0.0f, -1.0f, 0.0f), m_ground, 0.1f);
    }

    if (hitCount > 0)
    {
        // 法線と押し戻し量を平均化
        Vector3 avgNormal = totalNormal.normalized();
        float avgPenetration = totalPenetration / hitCount;

        // 位置補正（押し戻し）
        rb->SetPositionNext(rb->GetPositionNext() + avgNormal * avgPenetration);
        // 速度補正（衝突方向成分を消す）
        float vDotN = rb->GetVelocity().dot(avgNormal);
        if (vDotN < 0.0f)
        {
            rb->SetVelocity(rb->GetVelocity() - avgNormal * vDotN);
        }
    }

    m_Position = rb->GetPositionNext();


}

//回復
void Player::Heal()
{
    static float count = 0.0f;
    static int life = m_Life;

    if (m_Life < m_MaxLife)
    {
        if (m_Life < 0)
        {
            m_DrawEnable = false;
        }

        if (m_Life != life)
        {
            count = 0.0f;
        }

        if (count >= 300.0f)
        {
            m_Life++;
            count = 0.0f;
        }

        count++;
    }
    else
    {
        count = 0.0f;
    }

    life = m_Life;
}

//帰還
void Player::SetReturn()
{
    m_Position = m_ReSpawnPosition;
	auto* rb = GetComponent<Rigidbody>();
	rb->SetPositionNext(m_ReSpawnPosition);
    GetComponent<Rigidbody>()->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
}

//ジャンプフラグ
void Player::SetIsJump(bool jump)
{
    m_IsJumping = jump;
    if (jump)
    {
        m_SEjump->Play();
    }
}

//移動
const Vector3 Player::GetMoveInputFromCamera()
{
    auto* rb = GetComponent<Rigidbody>();

    //カメラ
    Camera* camera = Manager::GetScene()->GetGameObject<Camera>();

    if (!camera)
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    Vector3 camera_angle = camera->GetRotation();

    switch (camera->GetMode())
    {
    case Camera::ThirdPerson_Free:
        if (!m_Wire->GetExtending())
        {
            rb->SetAngularVelocity(Vector3(0.0f, (m_TargetRotation.y - m_Rotation.y) * 0.2f, 0.0f));
        }
        break;
    case Camera::ThirdPerson_Shooting:
        Vector3 fwd = camera->GetForward();
        fwd.y = 0;
        fwd.normalize();
        float camYaw = atan2f(fwd.x, fwd.z);
        camera_angle.y = camYaw;
        break;
    }

    // InputSystem
    float lx = GameInput::GetMoveX();
    float ly = GameInput::GetMoveY();

    Vector3 moveInput(0.0f, 0.0f, 0.0f);

    // カメラ基準方向
    Vector3 forward = camera->GetForward();
    forward.y = 0.0f;
    if (forward.lengthSq() > 0.0f)
    {
        forward.normalize();
    }

    Vector3 right = camera->GetRight();
    right.y = 0.0f;
    if (right.lengthSq() > 0.0f)
    {
        right.normalize();
    }

    // 入力方向をワールド方向へ変換
    moveInput = forward * ly + right * lx;

    //アナログ移動
    float inputLength = moveInput.length();

    if (inputLength > 0.0001f)
    {
        if (inputLength > 1.0f)
        {
            moveInput.normalize();
        }

        m_TargetRotation.y = atan2f(moveInput.x, moveInput.z);
        return Vector3(moveInput.x, 0.0f, moveInput.z);
    }

    return Vector3(0.0f, 0.0f, 0.0f);
}

//ワイヤロープシュート
void Player::WireShot()
{
    auto& targets = Manager::GetScene()->GetGameObjects(5);

    if (!targets.empty())
    {
        targets.sort([&](GameObject* a, GameObject* b) {return a->GetDistance(m_Position) < b->GetDistance(m_Position); });
        std::vector<GameObject*> validObjects;
        auto targetMark = Manager::GetScene()->GetGameObject<TargetMark>();

        for (auto target : targets)
        {
            if (target->GetDistance(m_Position) > m_EffDistance)
                continue;

            auto* wireComp = target->GetComponent<WireComponent>();
            if (!wireComp)
                continue;

            if (!wireComp->CanBeWired())
                continue;

            validObjects.push_back(target);
        }

        if (!validObjects.empty())
        {
            static int selectedIndex = 0;
            int anchorCount = static_cast<int>(validObjects.size());

            //範囲内の最大値が変化する時
            if (selectedIndex >= anchorCount)
            {
                selectedIndex = 0;

            }

            if (anchorCount > 0)
            {
                if (IsWirePrevTrigger())
                {
                    selectedIndex = (selectedIndex - 1 + anchorCount) % anchorCount;
                }
                if (IsWireNextTrigger())
                {
                    selectedIndex = (selectedIndex + 1) % anchorCount;
                }
            }

            if (IsWireShotTrigger())
            {
                if (IsStateWire())
                {
                    return;
                }

                m_SE->Play();
                GameObject* target = validObjects[selectedIndex];
                auto* wireComp = target->GetComponent<WireComponent>();

                if (!wireComp || !wireComp->CanBeWired())
                    return;

                Vector3 toTarget = target->GetPosition() - m_Position;
                toTarget.y = 0;
                toTarget.normalized();

                float yaw = atan2f(toTarget.x, toTarget.z);
                SetRotation(Vector3(0, yaw, 0));
                
                m_Wire->Set(m_Position, target->GetPosition(), this, target);
                m_TargetObject = target;
                wireComp->SetAttached();

                if (m_TargetObject->GetTag() == "ENEMY")
                {
                    ChangeState(new PlayerStateAssassin());
                    return;
                }
                else
                {
                    ChangeState(new PlayerStateWire());
                    return;
                }
                
            }

            for (int i = 0; i < anchorCount; i++)
            {
                if (auto* wireComp = validObjects[i]->GetComponent<WireComponent>())
                {
                    wireComp->Clear();
                }
            }

            if (targetMark)
            {
                targetMark->SetTarget(validObjects[selectedIndex]);
            }

            if (auto* wireComp = validObjects[selectedIndex]->GetComponent<WireComponent>())
            {
                wireComp->SetTargeted();
            }
        }
        else
        {
            if (IsWireShotTrigger())
            {
                if (IsStateWire())
                {
                    return;
                }
            }

            if (targetMark)
            {
                targetMark->SetTarget(nullptr);
            }
        }
    }
}

//ワイヤロープ解除
void Player::WireUnSet()
{
    m_Wire->UnSet();
    if (m_TargetObject)
    {
        if (auto* wire = m_TargetObject->GetComponent<WireComponent>())
        {
            wire->Clear();
        }
    }

    if (!m_IsGrounded)
    {
        m_IsJumping = false;
        ChangeState(new PlayerStateAir());
    }
    else
    {
        ChangeState(new PlayerStateGround());
    }
}

//弾シュート
void Player::BulletShot()
{
    if (IsBulletShotTrigger())
    {
        auto* camera = Manager::GetScene()->GetGameObject<Camera>();

        if (m_BulletNum > 0)
        {
            auto* bullet = Manager::GetScene()->AddGameObject<Bullet>(5);
            switch (camera->GetMode())
            {
            case Camera::ThirdPerson_Free:
            {
                bullet->Shot(m_Position + GetForward() * 2.0f, Vector3(0.1f, 0.1f, 0.1f), GetForward() * 30.0f);
                auto* bulletWireComp = bullet->GetComponent<WireComponent>();
                bulletWireComp->SetWireable(false);

                m_BulletNum--;
                break;
            }
            case Camera::ThirdPerson_Shooting:
            {
                //カメラの forward を使ってレイを作る
                Vector3 camPos = camera->GetPosition();
                Vector3 camDir = camera->GetForward();

                Line ray(camPos, camDir, 1000.0f);

                auto shapes = Manager::GetCollision()->ShapeCollision(ray);
                Vector3 target;
                if (!shapes.empty())
                {
                    Shape* closestShape = nullptr;
                    float closestDist = FLT_MAX;

                    for (auto shape : shapes)
                    {
                        if (!shape || !shape->GetpOwner())
                        {
                            continue;
                        }

                        if (shape->GetpOwner()->GetTag() == "COORDINATE"
                            || shape->GetpOwner()->GetTag() == "GROUND"
                            || shape->GetpOwner()->GetTag() == "MESHFIELD"
                            || shape->GetpOwner()->GetTag() == "WAVE"
                            || shape->GetpOwner()->GetTag() == "DISCOVERYAREA")
                        {
                            continue;
                        }

                        Vector3 hitPoint;
                        float hitDist;

                        if (!shape->Intersect(ray, 0, &hitPoint, &hitDist))
                        {
                            continue;
                        }

                        if (hitDist < closestDist)
                        {
                            closestDist = hitDist;
                            closestShape = shape;
                        }
                    }

                    if (closestShape)
                    {
                        target = closestShape->GetPosition();
                    }
                    else
                    {
                        target = camPos + camDir * 30.0f;
                    }
                }
                else
                {
                    target = camPos + camDir * 30.0f;
                }

                //レイヤー位置 → ターゲット の向きを発射方向にする
                Vector3 shootDir = (target - (m_Position + camera->GetForward() * 2.0f)).normalized();

                bullet->Shot(m_Position + camera->GetForward() * 2.0f, Vector3(0.1f, 0.1f, 0.1f), shootDir * 30.0f);
                auto* bulletWireComp = bullet->GetComponent<WireComponent>();
                bulletWireComp->SetWireable(false);

                m_BulletNum--;
                break;
            }
            }
        }
    }
}

//アニメションブレンド
void Player::AnimationBlend(std::string AnimationName)
{
    if (m_AnimationNameNext != AnimationName)
    {
        m_AnimationName = m_AnimationNameNext;
        m_AnimationNameNext = AnimationName;
        m_AnimationBlend = 0.0f;
    }
}

bool Player::IsStateWire() const
{
    return dynamic_cast<PlayerStateWire*>(m_CurrentState) != nullptr;
}

//無敵
void Player::SetInvincible()
{
    ChangeState(new PlayerStateInvincible);
}

//無敵フラグ
bool Player::IsStateInvincible() const
{
    return dynamic_cast<PlayerStateInvincible*>(m_CurrentState) != nullptr;
}

bool Player::IsStateClear() const
{
    return dynamic_cast<PlayerStateClear*>(m_CurrentState) != nullptr;
}

bool Player::IsStateDead() const
{
    return dynamic_cast<PlayerStateDead*>(m_CurrentState) != nullptr;
}

bool Player::IsStateCreater() const
{
    return dynamic_cast<PlayerStateCreater*>(m_CurrentState) != nullptr;
}

//入力
bool Player::IsJumpTrigger() const
{
    return GameInput::GetJumpTrigger();
}

bool Player::IsBulletShotTrigger() const
{
    return GameInput::GetAttackTrigger();
}

bool Player::IsWirePrevTrigger() const
{
	return GameInput::GetWirePrevTrigger();
}

bool Player::IsWireNextTrigger() const
{
	return GameInput::GetWireNextTrigger();
}

bool Player::IsWireShotTrigger() const
{
	return GameInput::GetWireShotTrigger();
}

void Player::Serialize(Serializer& s)
{
	GameObject::Serialize(s);
	s.WriteVector3("startPosition", m_StartPosition);
	s.WriteVector3("startRotation", m_StartRotation);
}

void Player::Deserialize(Serializer& s)
{
	GameObject::Deserialize(s);
	m_StartPosition = s.ReadVector3("startPosition");
	m_StartRotation = s.ReadVector3("startRotation");
}





