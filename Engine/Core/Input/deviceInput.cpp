/*==============================================================================

	ì¸óÕÉfÉoÉCÉXä«óù[deviceInput.cpp]
															 Author:äsÅ@ìNâF
															 Date  :2026/03/21
==============================================================================*/
#include "main.h"
#include "Engine/Core/Input/deviceInput.h"

bool DeviceInput::m_IsActive = true;

// Keyboard
BYTE DeviceInput::m_OldKeyState[256];
BYTE DeviceInput::m_KeyState[256];

// Gamepad
SDL_Gamepad* DeviceInput::m_Gamepad = nullptr;
bool DeviceInput::m_PadButtons[SDL_GAMEPAD_BUTTON_COUNT] = {};
bool DeviceInput::m_OldPadButtons[SDL_GAMEPAD_BUTTON_COUNT] = {};

float DeviceInput::m_LeftStickX = 0.0f;
float DeviceInput::m_LeftStickY = 0.0f;
float DeviceInput::m_RightStickX = 0.0f;
float DeviceInput::m_RightStickY = 0.0f;
float DeviceInput::m_LeftTrigger = 0.0f;
float DeviceInput::m_RightTrigger = 0.0f;
float DeviceInput::m_OldLeftTrigger = 0.0f;
float DeviceInput::m_OldRightTrigger = 0.0f;


void DeviceInput::UpdateKeyboard()
{
	memcpy(m_OldKeyState, m_KeyState, 256);
	GetKeyboardState(m_KeyState);
}

void DeviceInput::UpdateGamepad()
{
	m_OldLeftTrigger = m_LeftTrigger;
	m_OldRightTrigger = m_RightTrigger;

	std::memcpy(m_OldPadButtons, m_PadButtons, sizeof(m_PadButtons));

	// SDLì‡ïîÇÃèÛë‘çXêV
	SDL_PumpEvents();

	if (!m_Gamepad)
	{
		TryOpenGamepad();
	}

	if (!m_Gamepad)
	{
		std::memset(m_PadButtons, 0, sizeof(m_PadButtons));
		m_LeftStickX = 0.0f;
		m_LeftStickY = 0.0f;
		m_RightStickX = 0.0f;
		m_RightStickY = 0.0f;
		m_LeftTrigger = 0.0f;
		m_RightTrigger = 0.0f;
		return;
	}

	for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++)
	{
		m_PadButtons[i] = SDL_GetGamepadButton(m_Gamepad, static_cast<SDL_GamepadButton>(i));
	}

	m_LeftStickX = NormalizeStick(SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_LEFTX));
	m_LeftStickY = NormalizeStick(SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_LEFTY));
	m_RightStickX = NormalizeStick(SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_RIGHTX));
	m_RightStickY = NormalizeStick(SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_RIGHTY));

	m_LeftTrigger = NormalizeTrigger(SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER));
	m_RightTrigger = NormalizeTrigger(SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER));

}

void DeviceInput::TryOpenGamepad()
{
	int numJoysticks = 0;
	SDL_JoystickID* ids = SDL_GetJoysticks(&numJoysticks);

	if (!ids)
	{
		return;
	}

	for (int i = 0; i < numJoysticks; i++)
	{
		if (SDL_IsGamepad(ids[i]))
		{
			m_Gamepad = SDL_OpenGamepad(ids[i]);
			if (m_Gamepad)
			{
				OutputDebugStringA("Gamepad Connected\n");
				break;
			}
		}
	}

	SDL_free(ids);
}

void DeviceInput::CloseGamepad()
{
	if (m_Gamepad)
	{
		SDL_CloseGamepad(m_Gamepad);
		m_Gamepad = nullptr;
	}
}

float DeviceInput::NormalizeStick(Sint16 value, int deadZone)
{
	if (value > -deadZone && value < deadZone)
	{
		return 0.0f;
	}

	float f = static_cast<float>(value);

	if (f < 0.0f)
	{
		return f / 32768.0f;
	}

	return f / 32767.0f;
}

float DeviceInput::NormalizeTrigger(Sint16 value)
{
	if (value < 0)
	{
		value = 0;
	}

	return static_cast<float>(value) / 32767.0f;
}

void DeviceInput::Init()
{
	memset(m_KeyState, 0, sizeof(m_KeyState));
	memset(m_OldKeyState, 0, sizeof(m_OldKeyState));

	memset(m_PadButtons, 0, sizeof(m_PadButtons));
	memset(m_OldPadButtons, 0, sizeof(m_OldPadButtons));

	m_LeftStickX = 0.0f;
	m_LeftStickY = 0.0f;
	m_RightStickX = 0.0f;
	m_RightStickY = 0.0f;
	m_LeftTrigger = 0.0f;
	m_RightTrigger = 0.0f;

	// SDL3: ê¨å˜Ç≈ true / é∏îsÇ≈ false
	if (!SDL_Init(SDL_INIT_GAMEPAD))
	{
		OutputDebugStringA("SDL_Init(SDL_INIT_GAMEPAD) Failed\n");
		OutputDebugStringA(SDL_GetError());
		OutputDebugStringA("\n");
		return;
	}

	TryOpenGamepad();

}

void DeviceInput::Uninit()
{
	CloseGamepad();
	SDL_Quit();
}

void DeviceInput::Update()
{
	if (!m_IsActive)
	{
		ClearState();
		return;
	}

	UpdateKeyboard();
	UpdateGamepad();
}

bool DeviceInput::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool DeviceInput::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool DeviceInput::GetPadPress(SDL_GamepadButton button)
{
	if (button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT)
	{
		return false;
	}

	return m_PadButtons[button];
}

bool DeviceInput::GetPadTrigger(SDL_GamepadButton button)
{
	if (button < 0 || button >= SDL_GAMEPAD_BUTTON_COUNT)
	{
		return false;
	}

	return m_PadButtons[button] && !m_OldPadButtons[button];
}

float DeviceInput::GetLeftStickX()
{
	return m_LeftStickX;
}

float DeviceInput::GetLeftStickY()
{
	return m_LeftStickY;
}

float DeviceInput::GetRightStickX()
{
	return m_RightStickX;
}

float DeviceInput::GetRightStickY()
{
	return m_RightStickY;
}

float DeviceInput::GetLeftTrigger()
{
	return m_LeftTrigger;
}

float DeviceInput::GetRightTrigger()
{
	return m_RightTrigger;
}

bool DeviceInput::GetLeftTriggerPress(float threshold)
{
	return m_LeftTrigger > threshold;
}

bool DeviceInput::GetRightTriggerPress(float threshold)
{
	return m_RightTrigger > threshold;
}

bool DeviceInput::GetLeftTriggerTrigger(float threshold)
{
	return (m_LeftTrigger > threshold) && (m_OldLeftTrigger <= threshold);
}

bool DeviceInput::GetRightTriggerTrigger(float threshold)
{
	return (m_RightTrigger > threshold) && (m_OldRightTrigger <= threshold);
}

bool DeviceInput::IsGamepadConnected()
{
	return m_Gamepad != nullptr;
}

void DeviceInput::SetActive(bool active)
{
	m_IsActive = active;

	if (!active)
	{
		ClearState();
	}
}

void DeviceInput::ClearState()
{
	memset(m_KeyState, 0, sizeof(m_KeyState));
	memset(m_OldKeyState, 0, sizeof(m_OldKeyState));

	memset(m_PadButtons, 0, sizeof(m_PadButtons));
	memset(m_OldPadButtons, 0, sizeof(m_OldPadButtons));

	m_LeftStickX = 0.0f;
	m_LeftStickY = 0.0f;
	m_RightStickX = 0.0f;
	m_RightStickY = 0.0f;
	m_LeftTrigger = 0.0f;
	m_RightTrigger = 0.0f;
}

