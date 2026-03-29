/*==============================================================================

	ƒvƒŒƒCƒ„[_ƒgƒ“ƒlƒ‹[playerStateTunnel.cpp]
															 Author:Šs@“N‰F
															 Date  :2025/01/23
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "playerStateTunnel.h"

void PlayerStateTunnel::Enter(Player* player)
{
	auto* rb = player->GetComponent<Rigidbody>();

	rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));

	player->AnimationBlend("Idle");
}

void PlayerStateTunnel::Update(Player* player, float deltatime)
{

}

void PlayerStateTunnel::Exit(Player* player)
{

}