/*==============================================================================

	ƒvƒŒƒCƒ„[_’n–Ê[playerStateGround.h]
															 Author:Šs@“N‰F
															 Date  :2025/07/16
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "playerStateGround.h"
#include "playerStateAir.h"


void PlayerStateGround::Enter(Player* player)
{
    auto* rb = player->GetComponent<Rigidbody>();

    rb->SetMovementMode("Ground");
}

void PlayerStateGround::Update(Player* player, float deltatime)
{
    auto* rb = player->GetComponent<Rigidbody>();

    Vector3 moveInput = player->GetMoveInputFromCamera();

    rb->AddVelocity(moveInput);

    if (player->GetIsGrounded() && player->IsJumpTrigger())
    {
        player->SetIsJumpUp(true);
        player->SetIsJump(true);
        player->AnimationBlend("JumpUp");
        player->SetIsAnimationLoop(false);
        player->ReSetFrame();
    }

    if (player->GetIsJump() && !player->GetIsJumpUp())
    {
        player->ChangeState(new PlayerStateAir());
        return;
    }

    if (!player->GetIsJump())
    {
        if (player->GetMoveInputFromCamera().lengthSq() != 0.0f)
        {
            player->AnimationBlend("Run");
        }
        else
        {
            player->AnimationBlend("Idle");
        }
    }
    


    if (!player->GetIsGrounded())
    {
        player->ChangeState(new PlayerStateAir());
    }

    player->WireShot();
    player->BulletShot();
}

void PlayerStateGround::Exit(Player* player)
{
	
}
