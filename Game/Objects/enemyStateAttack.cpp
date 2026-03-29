/*==============================================================================

    “G_UŒ‚[enemyStateAttack.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/09/05
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/rigidbody.h"
#include "enemyStateAttack.h"
#include "enemyStateChase.h"
#include "player.h"
#include "particle.h"

void EnemyStateAttack::Enter(Enemy* enemy)
{
    enemy->SetMaterialColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    enemy->AnimationBlend("Attack");
    enemy->ReSetFrame();
    enemy->SetIsAttacking(true);
    enemy->CanBeWire(false);
}

void EnemyStateAttack::Update(Enemy* enemy, float deltatime)
{
    auto* rb = enemy->GetComponent<Rigidbody>();
    Player* player = dynamic_cast<Player*>(enemy->GetPlayer());
    Vector3 playerPos = player->GetPosition();

    Vector3 direction = playerPos - enemy->GetPosition();
    rb->SetVelocity(direction.normalized() * m_Speed);

    Vector3 rotation = enemy->GetRotation();
    enemy->SetRotation(Vector3(rotation.x, atan2f(direction.x, direction.z), rotation.z));

    if (!enemy->GetIsAttacking())
    {
        enemy->ChangeState(new EnemyStateChase());
        return;
    }

    if (player->IsStateDead())
    {
        enemy->ChangeState(new EnemyStateChase());
        return;
    }

    if (CapsuleToCapsule(*enemy->GetComponent<Capsule>(), *player->GetComponent<Capsule>()))
    {
        if (!player->IsStateInvincible())
        {
            Vector3 dir = (playerPos - enemy->GetPosition()).normalized();
            auto particle = Manager::GetScene()->AddGameObject<Particle>(4);
            particle->SetPosition(playerPos);
            particle->SetType(1);
            player->GetComponent<Rigidbody>()->SetVelocity(Vector3(dir.x * 400.0f, 20.0f, dir.z * 400.0f));
            player->SetInvincible();
            player->SetLifeDown(1);
        }
    }
}

void EnemyStateAttack::Exit(Enemy* enemy)
{
    auto* rb = enemy->GetComponent<Rigidbody>();
    rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
}