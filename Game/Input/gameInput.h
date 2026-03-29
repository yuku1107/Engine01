/*==============================================================================

	ÉQÅ[ÉÄì¸óÕä«óùÉNÉâÉX[gameInput.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2026/03/21
==============================================================================*/
#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include "Engine/Core/Input/inputTypes.h"

class GameInput
{
private:
    static float m_MouseSensitivity;
    static float m_GamepadSensitivity;

public:
    static void Init();

    static void SetMouseSensitivity(float sensitivity);
    static void SetGamepadSensitivity(float sensitivity);

    static float GetMoveX();
    static float GetMoveY();
    static InputVec2 GetMove();

    static float GetLookX();
    static float GetLookY();
    static InputVec2 GetLook();

    static bool GetAttackPressed();
    static bool GetAttackTrigger();

    static bool GetJumpPressed();
    static bool GetJumpTrigger();

    static bool GetAimPressed();

    static bool GetWireShotTrigger();

    static bool GetWireNextTrigger();
	static bool GetWireNextPressed();
    static bool GetWirePrevTrigger();
	static bool GetWirePrevPressed();

    static bool GetDiscoveryUITrigger();
};

#endif // GAME_INPUT_H