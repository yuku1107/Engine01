/*==============================================================================

    É^ÉCÉgÉãâÊñ [title.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/06/25
==============================================================================*/
#ifndef TITLE_H
#define TITLE_H

#include "Engine/Core/scene.h"

class Title :public Scene
{
private:
    bool m_Change = false;
    float m_Time = 0.0f;               
    float m_Amplitude = 0.5f;          
    float m_Speed = 2.0f;              

public:
    void Init() override;
    void Update() override;

    void StartEvent();

};

#endif //TITLE_H
