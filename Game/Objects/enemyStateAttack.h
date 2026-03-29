/*==============================================================================

    ìG_çUåÇ[enemyStateAttack.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/09/05
==============================================================================*/
#ifndef ENEMYSTATEATTACK_H
#define ENEMYSTATEATTACK_H

#include "enemy.h"

class EnemyStateAttack :public EnemyState
{
private:
    float m_Speed = 8.0f;
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltatime) override;
    void Exit(Enemy* enemy) override;
};


#endif //ENEMYSTATEATTACK_H