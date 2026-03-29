/*==============================================================================

    game_ÉXÉeÅ[ÉW0[gameStateStage0.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/09/12
==============================================================================*/
#ifndef GAMESTATESTAGE0_H
#define GAMESTATESTAGE0_H

#include "game.h"
#include "transferDevice.h"

class GameStateStage0 : public GameState
{
private:
    TransferDevice::TransferType m_NextStageType;
    bool m_ShouldChangeStage = false;
    class Audio* m_BGM;

public:
    void Enter(Game* game) override;
    void Update(Game* game, float deltatime) override;
    void Exit(Game* game) override;
};


#endif //GAMESTATESTAGE0_H
