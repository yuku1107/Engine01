/*==============================================================================

    ìG_çUåÇ[enemyStateAttackBullet.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/11/21
==============================================================================*/
#ifndef ENEMYSTATEATTACKBULLET_H
#define ENEMYSTATEATTACKBULLET_H

#include "enemy.h"

class EnemyStateAttackBullet :public EnemyState
{
private:
    float m_BulletSpeed = 8.0f;
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltatime) override;
    void Exit(Enemy* enemy) override;
};


#endif //ENEMYSTATEATTACKBULLET_H