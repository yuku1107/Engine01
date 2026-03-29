/*==============================================================================

    “G_Ž€–S[enemyStateDead.h]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/11/23
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "enemyStateSearch.h"
#include "enemyStateDead.h"
#include "player.h"


void EnemyStateDead::Enter(Enemy* enemy)
{
    auto* rb = enemy->GetComponent<Rigidbody>();
    enemy->SetMaterialColor({ 1.0f,1.0f,1.0f,1.0f });
    enemy->AnimationBlend("Idle");
    enemy->SetDrawEnable(false);
    enemy->SetPosition(Vector3(0.0f, -1000.0f, 0.0f));
    enemy->CanBeWire(false);
    rb->SetPositionNext(Vector3(0.0f, -1000.0f, 0.0f));
}

void EnemyStateDead::Update(Enemy* enemy, float deltatime)
{
    auto* rb = enemy->GetComponent<Rigidbody>();
    Player* player = dynamic_cast<Player*>(enemy->GetPlayer());
    Vector3 playerPos = player->GetPosition();
    Vector3 StartPosition= enemy->GetStartPosition();
    Vector3 StartRotation = enemy->GetStartRotation();
    Vector3 directionRe = playerPos - StartPosition;

    if (directionRe.length() >= enemy->GetReSpawnDistance())
    {
        enemy->SetLife(3);
        enemy->SetPosition(StartPosition);
        enemy->SetRotation(StartRotation);

        rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
		rb->SetPositionNext(StartPosition);
        enemy->ChangeState(new EnemyStateSearch());
    }
}

void EnemyStateDead::Exit(Enemy* enemy)
{

}