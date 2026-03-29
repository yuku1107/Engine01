/*==============================================================================

    プレイヤー_クリア[playerStateClear.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/09/15
==============================================================================*/
#ifndef PLAYERSTATCLEAR_H
#define PLAYERSTATCLEAR_H

#include "player.h"

class PlayerStateClear : public PlayerState
{
private:
    bool m_ClearReady = false;

public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltatime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATCLEAR_H
