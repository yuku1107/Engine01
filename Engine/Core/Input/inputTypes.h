/*==============================================================================

	入力共通定義[inputTypes.h]
                                                             Author:郭　哲宇
                                                             Date  :2026/03/21
==============================================================================*/
#ifndef INPUT_TYPES_H
#define INPUT_TYPES_H

#include <SDL3/SDL.h>

// アクション定義
enum class InputAction
{
    MoveX,
    MoveY,

    LookX,
    LookY,

    Attack,
    Jump,
    Aim,
    WireShot,
    WireNext,
    WirePrev,
    DiscoveryUI,
};

// 入力の種類
enum class BindingType
{
    Key,            // キーボード
    MouseButton,    // マウスボタン
    PadButton,      // パッドボタン
    PadAxis         // パッド軸
};

// アクションと入力の紐付け情報
struct InputBinding
{
    InputAction action;     // どのアクションか
    BindingType type;       // 入力種類
    int code;               // キーコードやボタン番号
    float scale;            // 軸の向きや倍率（A=-1, D=+1 など）
    float threshold;        // Trigger判定用しきい値（主にアナログ用）
};

struct InputVec2
{
    float x;
    float y;
};

#endif // INPUT_TYPES_H