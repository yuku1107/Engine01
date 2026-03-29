/*==============================================================================

    ìG_É^ÉCÉgÉãâÊñ [enemyStateTitle.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/12/12
==============================================================================*/
#ifndef ENEMYSTATETITLE_H
#define ENEMYSTATETITLE_H

#include "enemy.h"

class EnemyStateTitle :public EnemyState
{
private:
    Vector3 m_StartPosition;
    Vector3 m_StartRotation;
    bool m_IsGoingForward = true;
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltatime) override;
    void Exit(Enemy* enemy) override;
};


#endif //ENEMYSTATETITLE_H
