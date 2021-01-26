#include "playstate.h"
#include "game.h"
#include <iostream>

void PlayState::start() {
    mBoard.start();
}

void PlayState::handleEvents(sf::Event event) {
    if (!run) return;
    switch(event.type)
    {
    case sf::Event::Closed:
        mpGame->getWindow()->close();
        break;
    case sf::Event::KeyPressed:
        if(event.key.code == sf::Keyboard::Up)
            mUpPressed = true;
        if(event.key.code == sf::Keyboard::Space)
            mSpacePressed = true;
        if(event.key.code == sf::Keyboard::Left)
            mLeftPressed = true;
        if(event.key.code == sf::Keyboard::Right)
            mRightPressed = true;
        break;
    case sf::Event::Resized:
        // Spriječi deformaciju objekata prilikom promjene veličine prozora
        sf::FloatRect viewRect(0, 0, event.size.width, event.size.height);
        mpGame->getWindow()->setView(sf::View(viewRect));
        break;
    }
}

void PlayState::update() {
    if (!run)
    {
        run = true;
        vrijeme_1 = sf::Time::Zero;
        vrijeme_2 = sf::Time::Zero;
        c1.restart();
        c2.restart();
        mBoard.setSpeed(mpGame->mGameSpeed);
        mScore.resetScore();
    }

    if (mBoard.isGameOver())
    {
        run = false;
        mpGame->mGameScore=mScore.getScore();
        mpGame->setState(Speed);
    }

    vrijeme_1 +=c1.restart();
    vrijeme_2 +=c2.restart();
    if (mBoard.isGameOver())
    {
        //mScore.setGameOver();
        return;
    }
    int pomak=0;
    int rotacija=0;
    if (mUpPressed && rotacija==0) rotacija++;
    if (mLeftPressed && pomak==0) pomak--;
    if (mRightPressed && pomak==0) pomak++;
    if (mSpacePressed && pomak==0) pomak+=2;
    int score;
    if(mBoard.update(vrijeme_1, vrijeme_2, score, pomak, rotacija))
    {
        mUpPressed=false;
        mLeftPressed=false;
        mRightPressed=false;
        mSpacePressed=false;
    }
    mScore.addNoLinesCompleted(score);
    mScore.update();
}

void PlayState::render() {
    mpGame->getWindow()->draw(mBoard);
    mpGame->getWindow()->draw(mScore);
}

PlayState::PlayState(Game * pGame) : GameState(pGame) {

}
