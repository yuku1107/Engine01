/*==============================================================================

    ìG_éÄñS[enemyStateDead.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/11/23
==============================================================================*/
#ifndef ENEMYSTATEDEAD_H
#define ENEMYSTATEDEAD_H

#include "enemy.h"

class EnemyStateDead :public EnemyState
{
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltatime) override;
    void Exit(Enemy* enemy) override;
};


#endif //ENEMYSTATEDEAD_H
