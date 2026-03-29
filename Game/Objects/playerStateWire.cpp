/*==============================================================================

    プレイヤー_ワイヤー[playerStateWire.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/07/16
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "playerStateWire.h"
#include "playerStateAir.h"
#include "playerStateGround.h"
#include "playerStateWireBuster.h"
#include "playerStateWireBullet.h"
#include "wire.h"
#include "wireComponent.h"

void PlayerStateWire::Enter(Player* player)
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

void PlayerStateWire::Update(Player* player, float deltatime)
{
    auto* rb = player->GetComponent<Rigidbody>();
    Vector3 moveInput = player->GetMoveInputFromCamera();

    // --- 入力による移動（ワイヤー中も慣性制御）---
    rb->AddVelocity(moveInput);

    
    Wire* wire = player->GetWire();

    if (wire)
    {
        auto* target = player->GetTargetObject();

        if (!wire->GetExtending())
        {
            if (target->GetTag() == "BULLET")
            {
                if (player->GetBulletNum() == player->GetBulletMax())
                {
                    player->WireUnSet();
                    if (player->GetIsGrounded())
                    {
                        player->ChangeState(new PlayerStateGround);
                    }
                    else
                    {
                        player->ChangeState(new PlayerStateAir);
                    }
                    return;
                }

                wire->SetBuster(true);

                if(wire->GetMaxLength() <= 0.0f)
                {
                    player->ChangeState(new PlayerStateWireBullet);
                    return;
                }
            }
        }

        if (wire->GetEndObject()->GetDestroy())
        {
            player->WireUnSet();
            return;
        }

        if (wire->GetStartObject() != player)
        {
            return;
        }

        if (player->IsStateDead())
        {
            wire->UnSet();
            auto* targetWireComp = target->GetComponent<WireComponent>();
            targetWireComp->Clear();
            return;
        }

        auto targetRb = wire->GetEndObject()->GetComponent<Rigidbody>();

        // --- 移動モード判定 ---
        if (player->GetIsGrounded())
        {
            rb->SetMovementMode("Ground");
        }
        else if (wire->GetLinked())
        {
            if (targetRb)
            {
                if (rb->GetMass() < targetRb->GetMass())
                {
                    rb->SetMovementMode("Wire");
                }
            }
            else
            {
                rb->SetMovementMode("Wire");
            }

        }
        else
        {
            rb->SetMovementMode("Air");
        }

        Vector3 end = wire->GetEndPosition();
        Vector3 playerPos = player->GetPosition() + Vector3(0, 1.0f, 0);
        Vector3 offset = playerPos - end;
        Vector3 dir = offset.normalized();
        float maxLen = wire->GetMaxLength();

        if (wire->GetLinked())
        {
            player->SetPosition(rb->GetPositionNext());
            wire->ApplyPositionConstraint();
        }

        // ワイヤー解除（Vキー）
        if (player->IsWireShotTrigger())
        {
            player->WireUnSet();
            return;
        }

        if (player->IsJumpTrigger())
        {
            if (targetRb && rb->GetMass() < targetRb->GetMass())
            {
                if (!player->GetIsGrounded() && wire->GetLinked())
                {
                    wire->UnSet();
                    rb->SetVelocity(rb->GetVelocity() * 3.0f);
                    player->SetIsJump(true);
                    player->ChangeState(new PlayerStateAir());
                    return;
                }
            }
        }

        // ワイヤーバスター開始（Fキー）
        if (player->IsBulletShotTrigger())
        {
            wire->SetBuster(true);
            player->SetWireBusterPower(-dir * maxLen);
        }
    }


    if (wire->GetBuster() && wire->GetMaxLength() <= 0.0f)
    {
        player->ChangeState(new PlayerStateWireBuster());
        return;
    }

    if (player->GetIsGrounded() && player->IsJumpTrigger())
    {
        rb->SetMovementMode("Air");
        player->SetIsJump(true);
        rb->AddVelocity(player->GetJumpPower());
    }
}

void PlayerStateWire::Exit(Player* player)
{

}