/*==============================================================================

    “G_ƒ`ƒFƒCƒX[enemyStateChase.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/09/05
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "enemyStateChase.h"
#include "enemyStateAttack.h"
#include "enemyStateReturn.h"
#include "player.h"

void EnemyStateChase::Enter(Enemy* enemy)
{
    enemy->SetMaterialColor({ 1.0f,0.0f,0.0f,1.0f });
    enemy->AnimationBlend("Run");
    enemy->CanBeWire(false);
}

void EnemyStateChase::Update(Enemy* enemy, float deltatime)
{
    static int count = 0;
    Player* player = dynamic_cast<Player*>(enemy->GetPlayer());

    if (player->IsStateDead())
    {
        enemy->ChangeState(new EnemyStateReturn());
        return;
    }
    else
    {
        Vector3 playerPos = player->GetPosition();
        auto* rb = enemy->GetComponent<Rigidbody>();
        Vector3 direction = playerPos - enemy->GetPosition();
        rb->SetVelocity(direction.normalized() * m_Speed);

        float distance = direction.length();
        Vector3 rotation = enemy->GetRotation();
        Vector3 dir = direction.normalized();
        Vector3 forward = enemy->GetForward();
        const float cosFov = cosf(XMConvertToRadians(45.0f));
        float dot = forward.dot(dir);
        enemy->SetRotation(Vector3(rotation.x, atan2f(direction.x, direction.z), rotation.z));


        if (distance >= enemy->GetChaseDistance() || dot < cosFov)
        {
            count++;

            if (count >= 120 || player->IsStateDead())
            {
                enemy->ChangeState(new EnemyStateReturn());
                return;
            }
        }
        else if (distance <= enemy->GetAttackDistance())
        {
            enemy->ChangeState(new EnemyStateAttack());
            return;
        }
        else
        {
            count = 0;
        }
    }


}

void EnemyStateChase::Exit(Enemy* enemy)
{
    auto* rb = enemy->GetComponent<Rigidbody>();
    rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
}
