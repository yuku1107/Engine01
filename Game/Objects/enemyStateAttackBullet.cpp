/*==============================================================================

    “G_UŒ‚[enemyStateAttackBullet.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/11/21
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/rigidbody.h"
#include "enemyStateAttackBullet.h"
#include "enemyStateReturn.h"
#include "bullet.h"

void EnemyStateAttackBullet::Enter(Enemy* enemy)
{
    enemy->SetMaterialColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    enemy->AnimationBlend("Attack");
    enemy->ReSetFrame();
    enemy->SetIsAttacking(true);
    enemy->CanBeWire(false);
}

void EnemyStateAttackBullet::Update(Enemy* enemy, float deltatime)
{
    auto* rb = enemy->GetComponent<Rigidbody>();
    rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));

    if (!enemy->GetIsAttacking())
    {
        Vector3 direction;
        Bullet* bullet;

        bullet = Manager::GetScene()->AddGameObject<Bullet>(5);
        direction = enemy->GetForward() * m_BulletSpeed;
        bullet->Shot(enemy->GetPosition() + enemy->GetForward() * 2.0f, Vector3(0.5f, 0.5f, 0.5f), direction);

        enemy->ChangeState(new EnemyStateReturn());
    }
}

void EnemyStateAttackBullet::Exit(Enemy* enemy)
{
    auto* rb = enemy->GetComponent<Rigidbody>();
    rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
}