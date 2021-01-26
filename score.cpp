#include "score.h"
#include <cstdlib>
#include <iostream>
#include <string>

Score::Score()
{
    if(!mFont.loadFromFile("Gasalt-Regular.ttf")){
        std::cerr << "Ne mogu ucitati font: Gasalt-Regular.ttf !" << std::endl;
        std::exit(1);
    }

    std::string s="SCORE: ";
    s.append(std::to_string(mScore));
    mScoreText.setFont(mFont);
    mScoreText.setString(s);
    mScoreText.setPosition(300, 50);
}

void Score::resetScore(){
    mScore=0;
}

void Score::addNoLinesCompleted(int n)
{
    mScore+=10*n;
}

void Score::update()
{
    std::string s="SCORE: ";
    s.append(std::to_string(mScore));
    mScoreText.setString(s);
}

int Score::getScore() const
{
    return mScore;
}

void Score::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *=getTransform();
    target.draw(mScoreText);
}
