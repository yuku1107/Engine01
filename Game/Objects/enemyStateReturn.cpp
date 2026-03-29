/*==============================================================================

    ìG_ãAä“[enemyStateReturn.cpp]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/12/16
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "enemyStateReturn.h"
#include "enemyStateChase.h"
#include "enemyStateAttackBullet.h"
#include "enemyStateSearch.h"
#include "player.h"

void EnemyStateReturn::Enter(Enemy* enemy)
{
    m_StartPosition = enemy->GetStartPosition();
    m_StartRotation = enemy->GetStartRotation();
    enemy->SetMaterialColor({ 0.0f,1.0f,0.0f,1.0f });
    enemy->AnimationBlend("Run");
    enemy->CanBeWire(true);
    auto* rb = enemy->GetComponent<Rigidbody>();
    rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
    m_Return = true;
}

void EnemyStateReturn::Update(Enemy* enemy, float deltatime)
{
    Vector3 rotation = enemy->GetRotation();

    //ÉvÉåÉCÉÑÅ[
    Player* player = dynamic_cast<Player*>(enemy->GetPlayer());
    Vector3 playerPos = player->GetPosition();
    Vector3 direction = playerPos - enemy->GetPosition();
    float distance = direction.length();
    Vector3 dir = direction.normalized();
    Vector3 forward = enemy->GetForward();
    const float cosFov = cosf(XMConvertToRadians(45.0f));
    float dot = forward.dot(dir);

    if (distance <= enemy->GetChaseDistance() && dot >= cosFov && !player->IsStateDead())
    {
        enemy->SetRotation(Vector3(rotation.x, atan2f(direction.x, direction.z), rotation.z));
        switch (enemy->GetAttackType())
        {
        case 0:
            enemy->ChangeState(new EnemyStateChase());
            break;
        case 1:
            enemy->ChangeState(new EnemyStateAttackBullet());
            break;
        }

        m_Return = false;
    }
    else
    {
        m_Return = true;
    }
    
    if (m_Return)
    {
        //ãAä“
        auto* rb = enemy->GetComponent<Rigidbody>();
        Vector3 directionReturn = m_StartPosition - enemy->GetPosition();
        float distanceReturn = directionReturn.length();
        enemy->SetRotation(Vector3(rotation.x, atan2f(directionReturn.x, directionReturn.z), rotation.z));

        rb->SetVelocity(directionReturn.normalized() * m_Speed);

        if (distanceReturn <= 1.0f)
        {
            enemy->SetPosition(m_StartPosition);
            enemy->SetRotation(m_StartRotation);
            rb->SetPositionNext(m_StartPosition);
            rb->SetVelocity({0.0f,0.0f,0.0f});
            enemy->ChangeState(new EnemyStateSearch);
        }
    }

}


void EnemyStateReturn::Exit(Enemy* enemy)
{
    if (enemy->GetAttackType() == 1)
    {
        enemy->SetChaseDistance(20.0f);
    }
}