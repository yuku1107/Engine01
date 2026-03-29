/*==============================================================================

	入力マッピング管理[inputMapper.h]
                                                             Author:郭　哲宇
                                                             Date  :2026/03/21
==============================================================================*/
#ifndef INPUT_MAPPER_H
#define INPUT_MAPPER_H

#include <vector>
#include "inputTypes.h"

class InputMapper
{
private:
    static std::vector<InputBinding> m_Bindings;

private:
    static float GetBindingValue(const InputBinding& binding);

public:
    static void Init();
    static void SetupDefaultBindings();

    static void AddBinding(
        InputAction action,
        BindingType type,
        int code,
        float scale = 1.0f,
        float threshold = 0.5f
    );

    static void ClearBindings(InputAction action);
    static void ClearAllBindings();

    static float GetAxis(InputAction action);
    static bool GetButtonPressed(InputAction action);
    static bool GetButtonTrigger(InputAction action);
};

#endif // INPUT_MAPPER_H