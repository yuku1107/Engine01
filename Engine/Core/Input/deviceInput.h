/*==============================================================================

	入力デバイス管理[deviceInput.h]
                                                             Author:郭　哲宇
                                                             Date  :2026/03/21
==============================================================================*/
#ifndef DEVICE_INPUT_H
#define DEVICE_INPUT_H

#include <Windows.h>
#include <SDL3/SDL.h>

class DeviceInput
{
private:
    // Keyboard
    static BYTE m_OldKeyState[256]; // 前フレームのキーボード状態
    static BYTE m_KeyState[256];    // 現在フレームのキーボード状態

    static bool m_IsActive;         // 入力が有効かどうか（UI中などで無効化用）

    // Gamepad
    static SDL_Gamepad* m_Gamepad;  // 接続されているゲームパッド（1台想定）

    static bool m_PadButtons[SDL_GAMEPAD_BUTTON_COUNT];      // 現在のボタン状態
    static bool m_OldPadButtons[SDL_GAMEPAD_BUTTON_COUNT];   // 前フレームのボタン状態

    static float m_LeftStickX;   // 左スティックX軸（-1.0 ～ 1.0）
    static float m_LeftStickY;   // 左スティックY軸（-1.0 ～ 1.0）

    static float m_RightStickX;  // 右スティックX軸（-1.0 ～ 1.0）
    static float m_RightStickY;  // 右スティックY軸（-1.0 ～ 1.0）

    static float m_LeftTrigger;     // LT（0.0 ～ 1.0）
    static float m_RightTrigger;    // RT（0.0 ～ 1.0）
    static float m_OldLeftTrigger;  // 前フレームのLT値
    static float m_OldRightTrigger; // 前フレームのRT値

    static constexpr int PAD_DEADZONE = 8000; // スティックのデッドゾーン（微小入力無視）

private:
    static void UpdateKeyboard();   // キーボード状態を更新
    static void UpdateGamepad();    // ゲームパッド状態を更新

    static void TryOpenGamepad();   // 未接続時にゲームパッドを探して接続
    static void CloseGamepad();     // ゲームパッドを切断

    // スティック値を -1.0 ～ 1.0 に正規化（デッドゾーン適用）
    static float NormalizeStick(Sint16 value, int deadZone = PAD_DEADZONE);

    // トリガー値を 0.0 ～ 1.0 に正規化
    static float NormalizeTrigger(Sint16 value);

public:
    static void Init();
    static void Uninit();
    static void Update();

    // Keyboard取得
    // キーが押されているか（押しっぱなし）
    static bool GetKeyPress(BYTE KeyCode);

    // キーが押された瞬間（トリガー）
    static bool GetKeyTrigger(BYTE KeyCode);

    // Gamepad取得
    // ボタンが押されているか（押しっぱなし）
    static bool GetPadPress(SDL_GamepadButton button);

    // ボタンが押された瞬間
    static bool GetPadTrigger(SDL_GamepadButton button);

    static float GetLeftStickX();   // 左スティックX取得
    static float GetLeftStickY();   // 左スティックY取得

    static float GetRightStickX();  // 右スティックX取得
    static float GetRightStickY();  // 右スティックY取得

    static float GetLeftTrigger();  // LT取得（0.0～1.0）
    static float GetRightTrigger(); // RT取得（0.0～1.0）

    static bool GetLeftTriggerPress(float threshold = 0.5f);    // LT押されているか
    static bool GetRightTriggerPress(float threshold = 0.5f);   // RT押されているか

    static bool GetLeftTriggerTrigger(float threshold = 0.5f);  // LTが押された瞬間
    static bool GetRightTriggerTrigger(float threshold = 0.5f); // RTが押された瞬間

    // ゲームパッドが接続されているか
    static bool IsGamepadConnected();

    // 入力の有効/無効切り替え（falseで全入力停止）
    static void SetActive(bool active);

    // 入力状態をリセット（全て未入力状態に）
    static void ClearState();
};

#endif // DEVICE_INPUT_H