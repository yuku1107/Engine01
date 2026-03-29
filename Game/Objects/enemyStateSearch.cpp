/*==============================================================================

    “G_ƒT[ƒ`[enemyStateSearch.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/09/05
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "enemyStateSearch.h"
#include "enemyStateChase.h"
#include "enemyStateAttackBullet.h"
#include "player.h"

void EnemyStateSearch::Enter(Enemy* enemy)
{
    m_StartPosition = enemy->GetStartPosition();
    m_StartRotation =enemy->GetStartRotation();
    m_IsGoingForward = true;
    enemy->SetMaterialColor({ 1.0f,1.0f,1.0f,1.0f });
    enemy->AnimationBlend("Run");
    enemy->SetChaseDistance(10.0f);
    enemy->CanBeWire(true);
}

void EnemyStateSearch::Update(Enemy* enemy, float deltatime)
{
    Player* player = dynamic_cast<Player*>(enemy->GetPlayer());
    Vector3 playerPos = player->GetPosition();
    Vector3 direction = playerPos - enemy->GetPosition();
    float distance = direction.length();

    Vector3 dir = direction.normalized();
    Vector3 forward = enemy->GetForward();
    const float cosFov = cosf(XMConvertToRadians(45.0f));
    float dot = forward.dot(dir);

    auto* rb = enemy->GetComponent<Rigidbody>();
    Vector3 moveDir = enemy->GetForward().normalized();
    float distanceMove = (enemy->GetPosition() - m_StartPosition).length();
    float moveMax = enemy->GetMoveMax();

    if (m_IsGoingForward)
    {
        if (distanceMove > moveMax)
        {
            m_IsGoingForward = false;
            enemy->SetRotation(Vector3(m_StartRotation.x, m_StartRotation.y + XM_PI, m_StartRotation.z));
            moveDir = enemy->GetForward().normalized();
        }
    }
    else
    {
        if (distanceMove < 0.3f)
        {
            m_IsGoingForward = true;
            enemy->SetRotation(m_StartRotation);
            moveDir = enemy->GetForward().normalized();
        }
    }

    rb->SetVelocity(moveDir * m_Speed);

    if (distance <= enemy->GetChaseDistance() && dot >= cosFov && !player->IsStateDead())
    {
        enemy->SetRotation(Vector3(m_StartRotation.x, atan2f(direction.x, direction.z), m_StartRotation.z));
        switch (enemy->GetAttackType())
        {
        case 0:
            enemy->ChangeState(new EnemyStateChase());
            break;
        case 1:
            enemy->ChangeState(new EnemyStateAttackBullet());
            break;
        }
    }


}

void EnemyStateSearch::Exit(Enemy* enemy)
{

}