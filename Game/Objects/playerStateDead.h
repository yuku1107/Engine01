/*==============================================================================

    ÉvÉåÉCÉÑÅ[_éÄ[playerStateDead.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/09/15
==============================================================================*/
#ifndef PLAYERSTATEDEAD_H
#define PLAYERSTATEDEAD_H

#include "player.h"

class PlayerStateDead : public PlayerState
{
private:
    float m_ReturnCount = 0.0f;
    float m_ReturnSpeed = 1.0f;
    float m_ReturnMax = 5.0f;

public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltatime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATEDEAD_H
