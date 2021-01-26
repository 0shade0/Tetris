#include "gamestate.h"
#include "game.h"
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <sstream>

void WelcomeState::handleEvents(sf::Event event) {
    switch(event.type)
    {
    case sf::Event::Closed:
        mpGame->getWindow()->close();
        break;
    case sf::Event::Resized:
        // Sprijeèi deformaciju objekata prilikom promjene velièine prozora
        sf::FloatRect viewRect(0, 0, event.size.width, event.size.height);
        mpGame->getWindow()->setView(sf::View(viewRect));
        break;
    }
    if (event.type == sf::Event::KeyPressed)
        mpGame->setState(Playing);
}

void WelcomeState::update() {

}

void WelcomeState::render() {
    mpGame->getWindow()->draw(mText1);
    mpGame->getWindow()->draw(mText2);
}


WelcomeState::WelcomeState(Game * pGame) : GameState(pGame) {
    if(!mFont.loadFromFile("Gasalt-Regular.ttf")){
        std::cout << "Ne mogu ucitati font: Gasalt-Regular.ttf !" << std::endl;
        std::exit(1);
    }

    mText1.setFont(mFont);
    mText1.setString("Tetris clone by Damir Cupic");
    mText1.setCharacterSize(40);
    mText1.setPosition(sf::Vector2f(50, 200));

    mText2.setFont(mFont);
    mText2.setString("Press any key to continue");
    mText2.setCharacterSize(25);
    mText2.setPosition(sf::Vector2f(135, 240));
}

void SpeedState::start() {
    std::string text0="Score - ";
    text0.append(std::to_string(mpGame->mGameScore));
    mText0.setString(text0);



    std::string text1="Current speed = ";
    float speed=floorf(mpGame->mGameSpeed * 100) / 100;
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << speed;
    std::string s = stream.str();
    text1.append(s);
    mText1.setString(text1);
}

void SpeedState::handleEvents(sf::Event event) {
    switch(event.type)
    {
    case sf::Event::Closed:
        mpGame->getWindow()->close();
        break;
    case sf::Event::KeyPressed:
        if(event.key.code == sf::Keyboard::Up)
            mUpPressed = true;
        if(event.key.code == sf::Keyboard::Down)
            mDownPressed = true;
        if(event.key.code == sf::Keyboard::Enter)
            mpGame->setState(Playing);
        break;
    case sf::Event::Resized:
        // Sprijeèi deformaciju objekata prilikom promjene velièine prozora
        sf::FloatRect viewRect(0, 0, event.size.width, event.size.height);
        mpGame->getWindow()->setView(sf::View(viewRect));
        break;
    }
}

void SpeedState::update() {
    if (mUpPressed)
    {
        mpGame->mGameSpeed+=0.05f;
        mUpPressed=false;
    }
    if (mDownPressed)
    {
        mpGame->mGameSpeed-=0.05f;
        mDownPressed=false;
    }
    if (mpGame->mGameSpeed<=0.05f) mpGame->mGameSpeed=0.05f;

    std::string text="Current speed = ";
    float speed=floorf(mpGame->mGameSpeed * 100) / 100;
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << speed;
    std::string s = stream.str();
    text.append(s);
    mText1.setString(text);
}

void SpeedState::render() {
    mpGame->getWindow()->draw(mText0);
    mpGame->getWindow()->draw(mText1);
    mpGame->getWindow()->draw(mText2);
    mpGame->getWindow()->draw(mText3);
}

SpeedState::SpeedState(Game * pGame) : GameState(pGame) {
    if(!mFont.loadFromFile("Gasalt-Regular.ttf")){
        std::cout << "Ne mogu ucitati font: Gasalt-Regular.ttf !" << std::endl;
        std::exit(1);
    }

    mText0.setFont(mFont);
    mText0.setCharacterSize(25);
    mText0.setPosition(sf::Vector2f(220, 200));

    mText1.setFont(mFont);
    mText1.setCharacterSize(25);
    mText1.setPosition(sf::Vector2f(170, 225));

    mText2.setFont(mFont);
    mText2.setString("Use keys Up and Down to change to change the speed");
    mText2.setCharacterSize(20);
    mText2.setPosition(sf::Vector2f(55, 260));

    mText3.setFont(mFont);
    mText3.setString("Enter to continue");
    mText3.setCharacterSize(25);
    mText3.setPosition(sf::Vector2f(185, 285));
}
