/*==============================================================================

    敵_クリエーターモード[enemyStateCreater.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/02/02
==============================================================================*/
#ifndef ENEMYSTATECREATER_H
#define ENEMYSTATECREATER_H

#include "enemy.h"

class EnemyStateCreater :public EnemyState
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


#endif //ENEMYSTATECREATER_H
