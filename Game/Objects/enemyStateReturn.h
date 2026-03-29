/*==============================================================================

    ìG_ãAä“[enemyStateReturn.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/12/16
==============================================================================*/
#ifndef ENEMYSTATERETURN_H
#define ENEMYSTATERETURN_H

#include "enemy.h"

class EnemyStateReturn :public EnemyState
{
private:
    Vector3 m_StartPosition;
    Vector3 m_StartRotation;
    float m_Speed = 6.0f;
    bool m_Return;
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltatime) override;
    void Exit(Enemy* enemy) override;
};


#endif //ENEMYSTATERETURN_H
