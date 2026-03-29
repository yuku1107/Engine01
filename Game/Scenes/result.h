/*==============================================================================

    ƒŠƒUƒ‹ƒg‰æ–Ê[result.h]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/07/02
==============================================================================*/
#ifndef RESULT_H
#define RESULT_H

#include "Engine/Core/scene.h"

class Result :public Scene
{
private:
    bool m_Change = false;
    float m_Time = 0.0f;
    float m_Amplitude = 0.5f;
    float m_Speed = 2.0f;
public:
    void Init() override;
    void Update() override;

    void EndEvent();
};

#endif //RESULT_H

