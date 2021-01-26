#include "board.h"
#include "tetrimino.h"
#include <iostream>
#include <chrono>

// Vaš kod dolazi ovdje

void Board::start(){
    mGameOver=false;

    for (size_t i=0; i<mCols; ++i)
        for (size_t j=0; j<mRows; ++j)
            mGridContent[j][i]=-1;

    spawnPiece();
    placePiece();
}

Board::Board()
{


    sf::VertexArray horizontal_line(sf::Lines, mCols*2+2);
    sf::VertexArray vertical_line(sf::Lines, mRows*2+2);

    for (size_t i=0; i<=mCols; ++i)
    {
        horizontal_line[2*i].position=sf::Vector2f(mLeftMargin+mWidth*i/mCols, mTopMargin);
        horizontal_line[2*i].color=sf::Color::White;

        horizontal_line[2*i+1].position=sf::Vector2f(mLeftMargin+mWidth*i/mCols, mTopMargin+mHeight);
        horizontal_line[2*i+1].color=sf::Color::White;
    }

    for (size_t i=0; i<=mRows; ++i)
    {
        vertical_line[2*i].position=sf::Vector2f(mLeftMargin, mTopMargin+mHeight*i/mRows);
        vertical_line[2*i].color=sf::Color::White;

        vertical_line[2*i+1].position=sf::Vector2f(mLeftMargin+mWidth, mTopMargin+mHeight*i/mRows);
        vertical_line[2*i+1].color=sf::Color::White;
    }

    horizontal=horizontal_line;
    vertical=vertical_line;
}

void Board::setSpeed(float dt) { mDt=dt; }

bool Board::update(sf::Time & proteklo_vrijeme_1, sf::Time & proteklo_vrijeme_2, int & score_update, int x, int rot)
{

    sf::Time dt_1=sf::seconds(1/(mDt*15));
    sf::Time dt_2=sf::seconds(1/mDt);

    bool ret=false;
    bool space=false;

    while(proteklo_vrijeme_1 >= dt_1){
        proteklo_vrijeme_1 -= dt_1;
        if (x==1 || x==-1) movePiece(x,0);
        if (x==2)
        {
            space=true;
            movePiece(0, spaceToBottom());
        }
        if (rot) rotatePiece();
        ret=true;
    }
    while(proteklo_vrijeme_2 >= dt_2 && space==false){
        proteklo_vrijeme_2 -= dt_2;
        movePiece(0,1);
    }
    score_update=clearLines();
    if (score_update>0)
    {
        spawnPiece();
        placePiece();
    }
    if (space && ret) movePiece(0, 1);
    return ret;
}

void Board::spawnPiece()
{
    r.seed(std::chrono::system_clock::now().time_since_epoch().count());
    int type=r()%7;
    switch (type) {
        case 0: mPiece.mType=Tetrimino::O; break;
        case 1: mPiece.mType=Tetrimino::I; break;
        case 2: mPiece.mType=Tetrimino::S; break;
        case 3: mPiece.mType=Tetrimino::Z; break;
        case 4: mPiece.mType=Tetrimino::L; break;
        case 5: mPiece.mType=Tetrimino::J; break;
        case 6: mPiece.mType=Tetrimino::T; break;
    }
    type=r()%4;
    mPiece.mXposition=3;
    mPiece.mYposition=0;
    mPiece.mRotacija=type;
}

bool Board::isGameOver() const
{
    return mGameOver;
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *=getTransform();

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(mWidth/mCols, mHeight/mRows));
    for (auto i=0; i<mCols; i++)
        for (auto j=0; j<mRows; j++)
        {
            if (mGridContent[j][i]!=-1)
            {
                switch(mGridContent[j][i]) {
                case 0: rect.setFillColor(Tetrimino::mColors[Tetrimino::Types::O]); break;
                case 1: rect.setFillColor(Tetrimino::mColors[Tetrimino::Types::I]); break;
                case 2: rect.setFillColor(Tetrimino::mColors[Tetrimino::Types::S]); break;
                case 3: rect.setFillColor(Tetrimino::mColors[Tetrimino::Types::Z]); break;
                case 4: rect.setFillColor(Tetrimino::mColors[Tetrimino::Types::L]); break;
                case 5: rect.setFillColor(Tetrimino::mColors[Tetrimino::Types::J]); break;
                case 6: rect.setFillColor(Tetrimino::mColors[Tetrimino::Types::T]); break;
            }
            rect.setPosition(mLeftMargin+i*mWidth/mCols, mTopMargin+j*mHeight/mRows);
            target.draw(rect);
            }
        }
    target.draw(horizontal);
    target.draw(vertical);
}

void Board::placePiece()
{
    for (size_t i=0; i<4; i++)
    for (size_t j=0; j<4; j++)
        if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j]!=0)
            if(mGridContent[j][i+3]!=-1) mGameOver=true;

    if (!mGameOver)
    for (size_t i=0; i<4; i++)
    for (size_t j=0; j<4; j++)
        if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j]!=0)
            if(mGridContent[j+mPiece.mYposition][i+mPiece.mXposition]==-1) mGridContent[j+mPiece.mYposition][i+mPiece.mXposition]=mPiece.mType;
}

void Board::removePiece()
{
    for (size_t i=0; i<4; i++)
    for (size_t j=0; j<4; j++)
        if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j]!=0)  mGridContent[j+mPiece.mYposition][i+mPiece.mXposition]=-1;
}

bool Board::movePiece(int x, int y)
{
    int provjera = 0;

    removePiece();

    for (size_t i=0; i<4; i++)                                              //Provjeri izlazi li iz granica
    for (size_t j=0; j<4; j++)
        if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j]!=0)
        {
            if (mPiece.mYposition+j+y>=mRows || mPiece.mYposition+j+y<0) provjera++;
            if (mPiece.mXposition+i+x>=mCols || mPiece.mXposition+i+x<0) x=0;
        }

    for (size_t i=0; i<4; i++)                                              //Provjeri preklapanja sa drugim
    for (size_t j=0; j<4; j++)
        if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j]!=0
            && mGridContent[mPiece.mYposition+j+y][mPiece.mXposition+i+x]!=-1) provjera++;

    if (provjera==0)
    {
        for (size_t i=0; i<4; i++)                                              //Upiši tetrimino
        for (size_t j=0; j<4; j++)
            if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j]!=0)
                mGridContent[j+mPiece.mYposition+y][i+mPiece.mXposition+x]=mPiece.mType;
        mPiece.move(x,y);
        return true;
    }
    else
    {
        for (size_t i=0; i<4; i++)                                              //Vrati tetrimino
        for (size_t j=0; j<4; j++)
            if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j]!=0)
                mGridContent[j+mPiece.mYposition][i+mPiece.mXposition]=mPiece.mType;
        if (y>0)
        {
            spawnPiece();
            placePiece();
        }
        return false;
    }
}

int Board::spaceToBottom(){
    int space=1;
    int provjera=0;

    removePiece();

    while(!provjera)
    {
        for (size_t i=0; i<4; i++)                                              //Provjeri izlazi li iz granica
        for (size_t j=0; j<4; j++)
            if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j]!=0)
                if (mPiece.mYposition+j+space>=mRows || mPiece.mYposition+j+space<0) provjera++;

        for (size_t i=0; i<4; i++)                                              //Provjeri preklapanja sa drugim
        for (size_t j=0; j<4; j++)
            if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j]!=0
                && mGridContent[mPiece.mYposition+j+space][mPiece.mXposition+i]!=-1) provjera++;
        if (!provjera)
            space++;
        else return space-1;
    }
}

bool Board::rotatePiece()
{
    int provjera=0;
    removePiece();
    mPiece.rotate();
    for (size_t i=0; i<4; i++)
    for (size_t j=0; j<4; j++)
        if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j]!=0)
        {
            if (mPiece.mYposition+j>=mRows || mPiece.mYposition+j<0) provjera++;
            if (mPiece.mXposition+i>=mCols || mPiece.mXposition+i<0) provjera++;
            if (mGridContent[mPiece.mYposition+j][mPiece.mXposition+i]!=-1) provjera++;
        }

    if (provjera)
    {
        mPiece.rotate();
        mPiece.rotate();
        mPiece.rotate();
    }
    placePiece();
}

bool Board::isLineFull(int y) const
{
    int provjera=0;
    for (size_t i=0; i<mCols; i++) if (mGridContent[y][i]==-1) provjera++;
    return provjera ? false : true;
}

int Board::clearLines()
{
    int line_cleared=0;
    for (size_t z=0; z<mRows; z++)
        if (isLineFull(z))
        {
            for (size_t i=0; i<mCols; i++)
            for (size_t j=z; j>0; j--)
                mGridContent[j][i]=mGridContent[j-1][i];
            line_cleared++;
        }
    return line_cleared;
}
