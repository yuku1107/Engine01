/*==============================================================================

	ƒvƒŒƒCƒ„[_ƒŠƒUƒ‹ƒg‰æ–Ê[playerStateResult.cpp]
															 Author:Šs@“N‰F
															 Date  :2025/12/11
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "playerStateResult.h"

void PlayerStateResult::Enter(Player* player)
{
	auto* rb = player->GetComponent<Rigidbody>();

	rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));

	player->AnimationBlend("Byebye");
}

void PlayerStateResult::Update(Player* player, float deltatime)
{
	auto* rb = player->GetComponent<Rigidbody>();
	rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
	player->SetPosition(player->GetStartPosition());
	rb->SetPositionNext(player->GetPosition());
}

void PlayerStateResult::Exit(Player* player)
{

}