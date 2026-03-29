/*==============================================================================

    ìG_ÉäÉUÉãÉgâÊñ [enemyStateResult.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/12/12
==============================================================================*/
#ifndef ENEMYSTATERESULT_H
#define ENEMYSTATERESULT_H

#include "enemy.h"

class EnemyStateResult :public EnemyState
{
private:
    Vector3 m_StartPosition;
    Vector3 m_StartRotation;
public:
    void Enter(Enemy* enemy) override;
    void Update(Enemy* enemy, float deltatime) override;
    void Exit(Enemy* enemy) override;
};


#endif //ENEMYSTATERESULT_H
