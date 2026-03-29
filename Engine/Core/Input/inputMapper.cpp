/*==============================================================================

	ì¸óÕÉ}ÉbÉsÉìÉOä«óù[inputMapper.cpp]
															 Author:äsÅ@ìNâF
															 Date  :2026/03/21
==============================================================================*/
#include "main.h"
#include "Engine/Core/Input/inputMapper.h"
#include "Engine/Core/Input/deviceInput.h"
#include "Engine/Core/Input/Mouse/mouse.h"
#include <algorithm>
#include <cmath>

std::vector<InputBinding> InputMapper::m_Bindings;

void InputMapper::Init()
{
	ClearAllBindings();
	SetupDefaultBindings();
}

float InputMapper::GetBindingValue(const InputBinding& binding)
{
	switch (binding.type)
	{
	case BindingType::Key:
		return DeviceInput::GetKeyPress((BYTE)binding.code) ? binding.scale : 0.0f;

	case BindingType::MouseButton:
		return Mouse_IsPressed(static_cast<MouseButton>(binding.code)) ? binding.scale : 0.0f;

	case BindingType::PadButton:
		return DeviceInput::GetPadPress((SDL_GamepadButton)binding.code) ? binding.scale : 0.0f;

	case BindingType::PadAxis:
	{
		float value = 0.0f;

		switch (binding.code)
		{
		case SDL_GAMEPAD_AXIS_LEFTX:          value = DeviceInput::GetLeftStickX(); break;
		case SDL_GAMEPAD_AXIS_LEFTY:          value = DeviceInput::GetLeftStickY(); break;
		case SDL_GAMEPAD_AXIS_RIGHTX:         value = DeviceInput::GetRightStickX(); break;
		case SDL_GAMEPAD_AXIS_RIGHTY:         value = DeviceInput::GetRightStickY(); break;
		case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:   value = DeviceInput::GetLeftTrigger(); break;
		case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:  value = DeviceInput::GetRightTrigger(); break;
		default:                              value = 0.0f; break;
		}

		return value * binding.scale;
	}
	}

	return 0.0f;
}

void InputMapper::SetupDefaultBindings()
{
	m_Bindings.clear();

	// Move
	AddBinding(InputAction::MoveX, BindingType::Key, 'A', -1.0f);
	AddBinding(InputAction::MoveX, BindingType::Key, 'D', 1.0f);
	AddBinding(InputAction::MoveX, BindingType::PadAxis, SDL_GAMEPAD_AXIS_LEFTX, 1.0f);

	AddBinding(InputAction::MoveY, BindingType::Key, 'S', -1.0f);
	AddBinding(InputAction::MoveY, BindingType::Key, 'W', 1.0f);
	AddBinding(InputAction::MoveY, BindingType::PadAxis, SDL_GAMEPAD_AXIS_LEFTY, -1.0f);

	// Look
	AddBinding(InputAction::LookX, BindingType::PadAxis, SDL_GAMEPAD_AXIS_RIGHTX, 1.0f);
	AddBinding(InputAction::LookY, BindingType::PadAxis, SDL_GAMEPAD_AXIS_RIGHTY, 1.0f);

	// Attack
	AddBinding(InputAction::Attack, BindingType::Key, 'F');
	AddBinding(InputAction::Attack, BindingType::MouseButton, (int)MouseButton::Left);
	AddBinding(InputAction::Attack, BindingType::PadAxis, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, 1.0f, 0.5f);

	// Jump
	AddBinding(InputAction::Jump, BindingType::Key, VK_SPACE);
	AddBinding(InputAction::Jump, BindingType::PadButton, SDL_GAMEPAD_BUTTON_SOUTH);

	// Aim
	AddBinding(InputAction::Aim, BindingType::Key, VK_LSHIFT);
	AddBinding(InputAction::Aim, BindingType::PadAxis, SDL_GAMEPAD_AXIS_LEFT_TRIGGER, 1.0f, 0.5f);

	// Wire
	AddBinding(InputAction::WireShot, BindingType::Key, 'V');
	AddBinding(InputAction::WireShot, BindingType::PadButton, SDL_GAMEPAD_BUTTON_EAST);

	AddBinding(InputAction::WirePrev, BindingType::Key, 'X');
	AddBinding(InputAction::WirePrev, BindingType::PadButton, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);

	AddBinding(InputAction::WireNext, BindingType::Key, 'C');
	AddBinding(InputAction::WireNext, BindingType::PadButton, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);

	// DiscoveryUI
	AddBinding(InputAction::DiscoveryUI, BindingType::Key, 'Q');
	AddBinding(InputAction::DiscoveryUI, BindingType::PadButton, SDL_GAMEPAD_BUTTON_NORTH);
}

void InputMapper::AddBinding(InputAction action, BindingType type, int code, float scale, float threshold)
{
	m_Bindings.push_back({ action, type, code, scale, threshold });
}

void InputMapper::ClearBindings(InputAction action)
{
	for (auto it = m_Bindings.begin(); it != m_Bindings.end(); )
	{
		if (it->action == action)
			it = m_Bindings.erase(it);
		else
			++it;
	}
}

void InputMapper::ClearAllBindings()
{
	m_Bindings.clear();
}

float InputMapper::GetAxis(InputAction action)
{
	float value = 0.0f;

	for (const auto& binding : m_Bindings)
	{
		if (binding.action != action)
			continue;

		value += GetBindingValue(binding);
	}

	return Clamp(value, -1.0f, 1.0f);
}

bool InputMapper::GetButtonPressed(InputAction action)
{
	for (const auto& binding : m_Bindings)
	{
		if (binding.action != action)
			continue;

		switch (binding.type)
		{
		case BindingType::Key:
			if (DeviceInput::GetKeyPress((BYTE)binding.code))
				return true;
			break;

		case BindingType::MouseButton:
			if (Mouse_IsPressed(static_cast<MouseButton>(binding.code)))
				return true;
			break;

		case BindingType::PadButton:
			if (DeviceInput::GetPadPress((SDL_GamepadButton)binding.code))
				return true;
			break;

		case BindingType::PadAxis:
		{
			float value = GetBindingValue(binding);
			if (fabs(value) > binding.threshold)
				return true;
			break;
		}
		}
	}

	return false;
}

bool InputMapper::GetButtonTrigger(InputAction action)
{
	for (const auto& binding : m_Bindings)
	{
		if (binding.action != action)
			continue;

		switch (binding.type)
		{
		case BindingType::Key:
			if (DeviceInput::GetKeyTrigger((BYTE)binding.code))
				return true;
			break;

		case BindingType::MouseButton:
			if (Mouse_IsTrigger(static_cast<MouseButton>(binding.code)))
				return true;
			break;

		case BindingType::PadButton:
			if (DeviceInput::GetPadTrigger((SDL_GamepadButton)binding.code))
				return true;
			break;

		case BindingType::PadAxis:
		{
			switch (binding.code)
			{
			case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
				if (DeviceInput::GetLeftTriggerTrigger(binding.threshold))
					return true;
				break;

			case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
				if (DeviceInput::GetRightTriggerTrigger(binding.threshold))
					return true;
				break;
			}
			break;
		}
		}
	}

	return false;
}
