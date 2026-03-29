/*==============================================================================

	ƒvƒŒƒCƒ„[_‹ó’†[playerStateAir.cpp]
															 Author:Šs@“N‰F
															 Date  :2025/07/16
==============================================================================*/
#include "main.h"
#include "playerStateAir.h"
#include "playerStateGround.h"
#include "Engine/Components/rigidbody.h"

void PlayerStateAir::Enter(Player* player)
{
	auto* rb = player->GetComponent<Rigidbody>();

	rb->SetMovementMode("Air");
	
	if (player->GetIsJump())
	{
		rb->AddVelocity(player->GetJumpPower());
	}
}

void PlayerStateAir::Update(Player* player, float deltatime)
{
	auto* rb = player->GetComponent<Rigidbody>();

	Vector3 moveInput = player->GetMoveInputFromCamera();

	rb->AddVelocity(moveInput);

	if (player->GetIsGrounded() && rb->GetVelocity().y <= 0.0f)
	{
		player->SetIsJump(false);
		player->SetIsAnimationLoop(true);
		player->ChangeState(new PlayerStateGround());
	}

	player->WireShot();
	player->BulletShot();
}

void PlayerStateAir::Exit(Player* player)
{
	player->SetIsJump(false);
}
