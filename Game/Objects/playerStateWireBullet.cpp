/*==============================================================================

    プレイヤー_ワイヤーパレット[playerStateWireBullet.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/11/22
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "playerStateWireBullet.h"
#include "playerStateAir.h"
#include "playerStateGround.h"
#include "wire.h"
#include "bullet.h"

void PlayerStateWireBullet::Enter(Player* player)
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

void PlayerStateWireBullet::Update(Player* player, float deltatime)
{
    auto* bullet = player->GetTargetObject();

    bullet->SetDestroy();

    if (player->GetBulletNum() < player->GetBulletMax())
    {
        player->AddBulletNum();
    }

    player->WireUnSet();

    if (player->GetIsGrounded())
    {
        player->ChangeState(new PlayerStateGround);
    }
    else
    {
        player->ChangeState(new PlayerStateAir);
    }
}

void PlayerStateWireBullet::Exit(Player* player)
{

}
