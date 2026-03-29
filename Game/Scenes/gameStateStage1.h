/*==============================================================================

    game_ÉXÉeÅ[ÉW1[gameStateStage1.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/09/12
==============================================================================*/
#ifndef GAMESTATESTAGE1_H
#define GAMESTATESTAGE1_H

#include "game.h"

class GameStateStage1 : public GameState
{
private:
    bool m_ClearReady = false;
    bool m_Retry = true;
    bool m_ShouldChangeStage = false;
    class Audio* m_BGM;
public:
    void Enter(Game* game) override;
    void Update(Game* game, float deltatime) override;
    void Exit(Game* game) override;

    void ContinueEvent();
    void RetryEvent();
};


#endif //GAMESTATESTAGE1_H
