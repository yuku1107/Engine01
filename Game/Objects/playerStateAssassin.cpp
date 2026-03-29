/*==============================================================================

    ƒvƒŒƒCƒ„[_ˆÃŽE[playerStateAssassin.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/11/26
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/rigidbody.h"
#include "playerStateAssassin.h"
#include "playerStateAir.h"
#include "wire.h"
#include "enemy.h"
#include "particle.h"

void PlayerStateAssassin::Enter(Player* player)
{
    auto* rb = player->GetComponent<Rigidbody>();

    if (player->GetIsGrounded())
    {
        rb->SetMovementMode("Ground");
    }
    else
    {
        rb->SetMovementMode("Air");
    }
}

void PlayerStateAssassin::Update(Player* player, float delaytime)
{
    Wire* wire = player->GetWire();
    auto* rb = player->GetComponent<Rigidbody>();
    Enemy* enemy = dynamic_cast<Enemy*>(player->GetTargetObject());

    if (!enemy->IsStateSearch())
    {
        player->ChangeState(new PlayerStateAir());
        player->WireUnSet();
    }

    if (!wire->GetExtending())
    {

        if (wire->GetLinked())
        {
            player->SetPosition(rb->GetPositionNext());
            wire->ApplyPositionConstraint();
        }

        wire->SetBuster(true);

        if (wire->GetBuster() && wire->GetMaxLength() <= 0.0f)
        {
            player->ChangeState(new PlayerStateAir());
            player->WireUnSet();
            enemy->LifeDown(3);
            auto particle = Manager::GetScene()->AddGameObject<Particle>(4);
            particle->SetPosition(enemy->GetPosition());
            particle->SetType(1);
            return;
        }
    }

}

void PlayerStateAssassin::Exit(Player* player)
{

}
