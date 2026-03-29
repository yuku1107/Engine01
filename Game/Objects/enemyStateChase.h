/*==============================================================================

    ìG_É`ÉFÉCÉX[enemyStateChase.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/09/05
==============================================================================*/
#ifndef ENEMYSTATECHASE_H
#define ENEMYSTATECHASE_H

#include "enemy.h"

class EnemyStateChase :public EnemyState
{
private:
    float m_Speed = 10.0f;
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltatime) override;
    void Exit(Enemy* enemy) override;
};


#endif //ENEMYSTATECHASE_H
