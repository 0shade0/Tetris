#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include <SFML/Window/Event.hpp>

#include "gamestate.h"
#include "board.h"
#include "score.h"

// Ovo je bivša klasa Game. Doraditi.

class PlayState : public GameState
{
public:
    PlayState(Game * pGame);
    void  start() override;
private:
    Board mBoard;
    Score mScore;

    virtual void handleEvents(sf::Event event) override;
    virtual void update() override;
    virtual void render() override;

    bool mUpPressed    = false;
    bool mRightPressed = false;
    bool mLeftPressed  = false;
    bool mSpacePressed = false;

    bool run = false;

    sf::Clock c1, c2;
    sf::Time vrijeme_1, vrijeme_2;
};

#endif // PLAY_STATE_H
