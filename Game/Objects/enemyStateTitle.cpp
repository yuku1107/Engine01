/*==============================================================================

    “G_ƒ^ƒCƒgƒ‹‰æ–Ê[enemyStateTitle.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/12/12
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/rigidbody.h"
#include "enemyStateTitle.h"
#include "player.h"

void EnemyStateTitle::Enter(Enemy* enemy)
{
    m_StartPosition = enemy->GetStartPosition();
    m_StartRotation = enemy->GetStartRotation();
    m_IsGoingForward = true;
    enemy->SetMaterialColor({ 1.0f,1.0f,1.0f,1.0f });
    enemy->AnimationBlend("Run");
    enemy->CanBeWire(false);
}

void EnemyStateTitle::Update(Enemy* enemy, float deltatime)
{
    Vector3 forward = enemy->GetForward();
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

    if (!Manager::GetCreaterMode())
    {
        rb->SetVelocity(moveDir * 3.0f);
    }
    else
    {
        rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
    }
}

void EnemyStateTitle::Exit(Enemy* enemy)
{

}