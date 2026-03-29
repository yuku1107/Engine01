/*==============================================================================

    ’e[bullet.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/06/04
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Components/rigidbody.h"
#include "bullet.h"
#include "player.h"
#include "particle.h"
#include "enemy.h"
#include "wireComponent.h"

void Bullet::Init()
{
    m_Model = new AnimationModel;
    m_Model->Load("Assets\\model\\Collision_sphere.fbx");

    AddComponent<Sphere>()->Set(this);

    m_ShaderName = "unlitTexture";
    AddComponent<Shader>()->Set(this);

    auto rb = AddComponent<Rigidbody>();
    rb->Set(this);
    rb->SetMass(-1.0f);
    rb->SetUseGravity(false);

    AddComponent<WireComponent>();

    m_Tag = "BULLET";
}

void Bullet::UninitSelf()
{
    m_Model->Uninit();
    delete m_Model;
}

void Bullet::UpdateSelf()
{
    auto* player = Manager::GetScene()->GetGameObject<Player>();
    auto* rb = GetComponent<Rigidbody>();
    auto* sphere = GetComponent<Sphere>();
    auto* wireComp = GetComponent<WireComponent>();

    if (player)
    {
        if ((player->GetPosition() - m_Position).lengthSq() <= (player->GetEffDistance() * player->GetEffDistance()))
        {
            if (wireComp->IsTargeted())
            {
                m_MaterialColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
            }
            else
            {
                m_MaterialColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            }
        }
        else
        {
            wireComp->Clear();
            m_MaterialColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        if ((m_StartPosition - m_Position).length() > 30.0f)
        {
            SetDestroy();
        }
    }
    
    m_Position = rb->GetPositionNext();

    if (!wireComp->IsAttached())
    {
        auto shapes = Manager::GetCollision()->ShapeCollision(*sphere);

        if (shapes.size() != 0)
        {
            for (auto shape : shapes)
            {
                if (wireComp->CanBeWired())
                {
                    if (shape->GetpOwner()->GetTag() == "PLAYER")
                    {
                        Player* player = dynamic_cast<Player*>(shape->GetpOwner());
                        Vector3 playerPos = player->GetPosition();
                        if (!player->IsStateDead() && !player->IsStateInvincible())
                        {
                            Vector3 dir = (playerPos - m_Position).normalized();
                            auto particle = Manager::GetScene()->AddGameObject<Particle>(4);
                            particle->SetPosition(m_Position);
                            particle->SetType(1);
                            SetDestroy();
                            player->GetComponent<Rigidbody>()->SetVelocity(Vector3(dir.x * 400.0f, 20.0f, dir.z * 400.0f));
                            player->SetInvincible();
                            player->SetLifeDown(1);
                        }
                    }
                }
                else
                {
                    if (shape->GetpOwner()->GetTag() == "BULLET")
                    {
                        Bullet* bullet = dynamic_cast<Bullet*>(shape->GetpOwner());
                        auto* otherWireComp = bullet->GetComponent<WireComponent>();

                        if (!otherWireComp->IsAttached())
                        {
                            auto particle = Manager::GetScene()->AddGameObject<Particle>(4);
                            particle->SetPosition(m_Position);
                            particle->SetType(1);
                            SetDestroy();
                            bullet->SetDestroy();
                        }
                    }

                    if (shape->GetpOwner()->GetTag() == "ENEMY")
                    {
                        Enemy* enemy = dynamic_cast<Enemy*>(shape->GetpOwner());
                        auto particle = Manager::GetScene()->AddGameObject<Particle>(4);
                        particle->SetPosition(m_Position);
                        particle->SetType(1);
                        SetDestroy();
                        enemy->LifeDown(3);
                    }
                }
            }
        }
    }

}

void Bullet::DrawSelf()
{
    //ƒ}ƒgƒŠƒNƒXÝ’è
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y + XM_PI, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    world = scale * rotation * trancelation;

    Renderer::SetWorldMatrix(world);

    m_Model->Draw(m_MaterialColor);
}

void Bullet::Shot(Vector3 start, Vector3 scale, Vector3 velocity)
{
    m_Position = start;
    m_StartPosition = start;
    m_Scale = scale;
    GetComponent<Rigidbody>()->SetVelocity(velocity);
}
