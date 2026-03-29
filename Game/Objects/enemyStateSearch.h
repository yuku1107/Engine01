/*==============================================================================

    ìG_ÉTÅ[É`[enemyStateSearch.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/09/05
==============================================================================*/
#ifndef ENEMYSTATESEARCH_H
#define ENEMYSTATESEARCH_H

#include "enemy.h"

class EnemyStateSearch :public EnemyState
{
private:
    Vector3 m_StartPosition;
    Vector3 m_StartRotation;
    float m_Speed = 4.0f;
    bool m_IsGoingForward = true;
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltatime) override;
    void Exit(Enemy* enemy) override;
};


#endif //ENEMYSTATESEARCH_H
