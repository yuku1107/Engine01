/*==============================================================================

	ÉQÅ[ÉÄì¸óÕä«óùÉNÉâÉX[gameInput.cpp]
															 Author:äsÅ@ìNâF
															 Date  :2026/03/21
==============================================================================*/
#include "main.h"
#include "Engine/Core/Input/inputMapper.h"
#include "Engine/Core/Input/deviceInput.h"
#include "Engine/Core/Input/Mouse/mouse.h"
#include "Game/Input/gameInput.h"

float GameInput::m_MouseSensitivity;
float GameInput::m_GamepadSensitivity;

void GameInput::Init()
{
	m_MouseSensitivity = 0.002f;
	m_GamepadSensitivity = 0.06f;
}

void GameInput::SetMouseSensitivity(float sensitivity)
{
	m_MouseSensitivity = sensitivity;
}

void GameInput::SetGamepadSensitivity(float sensitivity)
{
	m_GamepadSensitivity = sensitivity;
}

float GameInput::GetMoveX()
{
	return InputMapper::GetAxis(InputAction::MoveX);
}

float GameInput::GetMoveY()
{
	return InputMapper::GetAxis(InputAction::MoveY);
}

InputVec2 GameInput::GetMove()
{
	return { GetMoveX(), GetMoveY() };
}


float GameInput::GetLookX()
{
	float mouse = Mouse_GetDelta().x * m_MouseSensitivity;
	float pad = DeviceInput::GetRightStickX() * m_GamepadSensitivity;

	return mouse + pad;
}

float GameInput::GetLookY()
{
	float mouse = Mouse_GetDelta().y * m_MouseSensitivity;
	float pad = DeviceInput::GetRightStickY() * m_GamepadSensitivity;

	return mouse + pad;
}

InputVec2 GameInput::GetLook()
{
	auto delta = Mouse_GetDelta();

	float mouseX = delta.x * m_MouseSensitivity;
	float mouseY = delta.y * m_MouseSensitivity;

	float padX = DeviceInput::GetRightStickX() * m_GamepadSensitivity;
	float padY = DeviceInput::GetRightStickY() * m_GamepadSensitivity;

	return { mouseX + padX, mouseY + padY };
}

bool GameInput::GetAttackPressed()
{
	return InputMapper::GetButtonPressed(InputAction::Attack);
}

bool GameInput::GetAttackTrigger()
{
	return InputMapper::GetButtonTrigger(InputAction::Attack);
}

bool GameInput::GetJumpPressed()
{
	return InputMapper::GetButtonPressed(InputAction::Jump);
}

bool GameInput::GetJumpTrigger()
{
	return InputMapper::GetButtonTrigger(InputAction::Jump);
}

bool GameInput::GetAimPressed()
{
	return InputMapper::GetButtonPressed(InputAction::Aim);
}

bool GameInput::GetWireShotTrigger()
{
	return InputMapper::GetButtonTrigger(InputAction::WireShot);
}

bool GameInput::GetWireNextTrigger()
{
	return InputMapper::GetButtonTrigger(InputAction::WireNext);
}

bool GameInput::GetWireNextPressed()
{
	return InputMapper::GetButtonPressed(InputAction::WireNext);
}

bool GameInput::GetWirePrevTrigger()
{
	return InputMapper::GetButtonTrigger(InputAction::WirePrev);
}

bool GameInput::GetWirePrevPressed()
{
	return InputMapper::GetButtonPressed(InputAction::WirePrev);
}

bool GameInput::GetDiscoveryUITrigger()
{
	return InputMapper::GetButtonTrigger(InputAction::DiscoveryUI);
}
