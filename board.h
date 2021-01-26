#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include "tetrimino.h"
#include <random>

/** Klasa Board
 *
 *  Klasa implementira svu logiku Tetris igre.
 */
class Board : public sf::Drawable, public sf::Transformable, public sf::NonCopyable {
public:
    Board();
    // Aktualizacija
    bool update(sf::Time & proteklo_vrijeme_1, sf::Time & proteklo_vrijeme_2, int & score_update, int x, int rot);
    // Pozicioniraj novi tetrimino na ploči.
    void spawnPiece();
    // Vraća true ako je igra gotova, inače false
    bool isGameOver() const;
    // Broj obrisanih linija od zadnje aktualizacije scene.
    void setSpeed(float dt);
    void start();
private:
    static const int EMPTY = -1;
    static const int mCols = 10;       // Broj stupaca u mreži
    static const int mRows = 2* mCols; // Broj redaka u mreži

    int spaceToBottom();

    sf::VertexArray horizontal;
    sf::VertexArray vertical;      // Linije koje čine mrežu.

    int mGridContent[mRows][mCols];    // Logički prikaz mreže

    // Dimenzije mreže
    float mLeftMargin = 50.f;
    float mTopMargin = 50.0f;
    float mWidth=200.f;
    float mHeight=2*mWidth;
    float mOriginX, mOriginY;
    float mDx, mDy;

    // Vremenski interval između propadanja tetrimina.
    float mDt = 0.3f; // sekunde
    // Signalizacija da je igra završena
    bool mGameOver = false;

    // Generiranje slučajnog elementa.
    std::default_random_engine r;

    // Komad koji pada
    Tetrimino mPiece;
    // Iscrtavanje
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    // Smjesti komad na ploču. Pretpostavlja se da je checkSpace(piece) vratio true.
    void placePiece();
    // Ukloni komad sa ploče.
    void removePiece();
    // Pomakni komad ako je moguće. Vrati true ako je pomak izvršen, inače false.
    bool movePiece(int x, int y);
    // Rotiraj komad. Vrati true ako je rotacija izvršena, inače false.
    bool rotatePiece();
    // Da li je linija y prazna ili nije ?
    bool isLineFull(int y) const;
    // Obriši kompletne linije i vrati broj obrisanih linija
    int clearLines();
    // Eventualno vaše dodatne metode dolaze ovdje.
    // Privatne metode koje ne trebate možete slobodno ukloniti.

};

#endif // BOARD_H
