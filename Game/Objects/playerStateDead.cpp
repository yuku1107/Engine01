/*==============================================================================

	ƒvƒŒƒCƒ„[_Ž€[playerStateDead.cpp]
															 Author:Šs@“N‰F
															 Date  :2025/09/15
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "playerStateDead.h"
#include "playerStateAir.h"


void PlayerStateDead::Enter(Player* player)
{
	auto* rb = player->GetComponent<Rigidbody>();

	rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));

	m_ReturnCount = 0.0f;
}

void PlayerStateDead::Update(Player* player, float deltatime)
{
	auto* rb = player->GetComponent<Rigidbody>();

	rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));

	m_ReturnCount += m_ReturnSpeed * deltatime;

	if (m_ReturnCount >= m_ReturnMax)
	{
		player->SetDrawEnable(true);
		player->SetReturn();
		player->ChangeState(new PlayerStateAir());
	}
}

void PlayerStateDead::Exit(Player* player)
{
	player->SetDrawEnable(true);
	player->SetLifeMax();
}