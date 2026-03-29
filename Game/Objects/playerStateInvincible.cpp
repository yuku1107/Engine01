/*==============================================================================

	ƒvƒŒƒCƒ„[_–³“G[playerStateInvincible.cpp]
															 Author:Šs@“N‰F
															 Date  :2025/12/16
==============================================================================*/
#include "main.h"
#include "Engine/Core/Input/input.h"
#include "Engine/Components/rigidbody.h"
#include "playerStateInvincible.h"
#include "playerStateAir.h"

void PlayerStateInvincible::Enter(Player* player)
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

void PlayerStateInvincible::Update(Player* player, float deltatime)
{
	auto* rb = player->GetComponent<Rigidbody>();

	Vector3 moveInput = player->GetMoveInputFromCamera();

	rb->AddVelocity(moveInput);

	m_Count--;
	int count = m_Count % 2;
	if (count == 0)
	{
		player->SetMaterialColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
	}
	else
	{
		player->SetMaterialColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}


	if (m_Count <= 0)
	{
		player->ChangeState(new PlayerStateAir);
	}
}

void PlayerStateInvincible::Exit(Player* player)
{
	m_Count = 90;
	player->SetMaterialColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}