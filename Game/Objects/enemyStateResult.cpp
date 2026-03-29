/*==============================================================================

    “G_ƒŠƒUƒ‹ƒg‰æ–Ê[enemyStateResult.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/12/12
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "enemyStateResult.h"

void EnemyStateResult::Enter(Enemy* enemy)
{
    m_StartPosition = enemy->GetStartPosition();
    m_StartRotation = enemy->GetStartRotation();
    enemy->SetMaterialColor({ 1.0f,1.0f,1.0f,1.0f });
    enemy->AnimationBlend("Idle");
    enemy->CanBeWire(false);
    auto* rb = enemy->GetComponent<Rigidbody>();
    rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));

}

void EnemyStateResult::Update(Enemy* enemy, float deltatime)
{

}

void EnemyStateResult::Exit(Enemy* enemy)
{

}