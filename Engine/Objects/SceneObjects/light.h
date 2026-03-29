/*==============================================================================

    ŒõŒ¹[light.h]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/06/22
==============================================================================*/
#ifndef LIGHT_H
#define LIGHT_H

#include "Engine/Core/gameObject.h"

class Light :public GameObject
{
private:
    LIGHT m_Light;
    class AnimationModel* m_Model;

public:
    void Init() override;   //‰Šú‰»
    void UninitSelf() override; //I—¹
    void UpdateSelf() override; //XV
    void DrawSelf() override;   //•`‰æ

    LIGHT* GetLight() { return &m_Light; }

    std::string GetObjectClassName() const override { return "Light"; }

};

#endif //LIGHT_H
