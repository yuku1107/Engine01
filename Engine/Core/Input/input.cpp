/*==============================================================================

	入力システム管理クラス[input.cpp]
															 Author:郭　哲宇
															 Date  :2025/05/14
==============================================================================*/
#include "main.h"
#include "Engine/Core/Input/input.h"
#include "Engine/Core/Input/deviceInput.h"
#include "Engine/Core/Input/inputMapper.h"

void Input::Init()
{
	DeviceInput::Init();
	InputMapper::Init();
}

void Input::Uninit()
{
	DeviceInput::Uninit();
	InputMapper::ClearAllBindings();
}

void Input::Update()
{
	DeviceInput::Update();
}


