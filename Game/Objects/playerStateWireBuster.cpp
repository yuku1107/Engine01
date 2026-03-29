/*==============================================================================

    プレイヤー_ワイヤーブースター[playerStateWireBuster.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/07/16
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "playerStateWireBuster.h"
#include "playerStateAir.h"
#include "wire.h"

void PlayerStateWireBuster::Enter(Player* player)
{
    Wire* wire = player->GetWire();
    auto* rb = player->GetComponent<Rigidbody>();

    rb->AddAcceleration(player->GetWireBusterPower() * Vector3(1000.0f, 1000.0f, 1000.0f));

    wire->UnSet();
    rb->SetMovementMode("Wire");
}

void PlayerStateWireBuster::Update(Player* player, float deltatime)
{
    static float count = 0;
    if (count > 2.0f)
    {
        player->ChangeState(new PlayerStateAir());
        count = 0;
    }
    else
    {
        count ++;
    }
}

void PlayerStateWireBuster::Exit(Player* player)
{

}
