#ifndef CANH_GRAPHIC_H
#define CANH_GRAPHIC_H

#include "board.h"
#include "util.h"
#include "timer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <exception>
#include <string>
#include <memory>

class Graphic
{
public:
    typedef uint32_t Size;
    typedef int32_t Pos;
    typedef SDL_Rect Rect;

    enum Sprite
    {
        CELL_ZERO = 0,
        CELL_ONE,
        CELL_TWO,
        CELL_THREE,
        CELL_FOUR,
        CELL_FIVE,
        CELL_SIX,
        CELL_SEVEN,
        CELL_EIGHT,
        CELL_MINE,
        CELL_MINE_WRONG,
        CELL_MINE_CURRENT,
        CELL_QUESTION_MARK,
        CELL_FLAG,
        CELL_UNOPENED,

        COUNT_ZERO,
        COUNT_ONE,
        COUNT_TWO,
        COUNT_THREE,
        COUNT_FOUR,
        COUNT_FIVE,
        COUNT_SIX,
        COUNT_SEVEN,
        COUNT_EIGHT,
        COUNT_NINE,

        EMOJI_SELECTING,
        EMOJI_PLAYING,
        EMOJI_CELL_SELECTING,
        EMOJI_LOST,
        EMOJI_WON,

        SPRITE_TOTAL
    };

    class Exception : public std::runtime_error
    {
    public:
        Exception(const std::string &msg) : std::runtime_error(msg) {}
    };

    static void showError(const std::string &);

    Graphic(const std::string &title, Size w, Size h);
    ~Graphic();

    void createBoard(Board::Size nRows, Board::Size nCols, Board::Size nMines,
        const Rect &boardRect);

    void createBanner(const Rect &bannerRect);

    void loop();

private:
    static const std::string SPRITE_PATH;

    static const Size CELL_W;
    static const Size CELL_H;
    static const Size COUNT_W;
    static const Size COUNT_H;
    static const Size EMOJI_W;
    static const Size EMOJI_H;

    static std::vector<SDL_Rect> SPRITE_RECTS;
    static Size s_NIns;

    static void initSpriteRects();

    SDL_Window *m_Window;
    SDL_Renderer *m_Renderer;
    SDL_Texture *m_SpriteTexture;
    bool m_RedrawRequired;
    double m_ScaleW;
    double m_ScaleH;

    std::unique_ptr<Board> m_Board;
    SDL_Rect m_BoardRect;
    bool m_BoardSelecting;
    Board::Pos m_BoardLastPos;

    SDL_Rect m_BannerRect;
    SDL_Rect m_EmojiRect;
    bool m_EmojiSelecting;

    Timer::Sec m_LastDrawSec;

    bool handleEvent(const SDL_Event &);
    void draw();

    void drawBoard() const;
    void drawCell(Board::Pos p, const Rect &spriteRect) const;
    void drawCellNeighborsOpening(Board::Pos) const;

    void drawRemainingNMines() const;
    void drawEmoji() const;
    void drawElapsedSec() const;

    Rect getSpriteRect(Board::Pos p) const;
    Board::Pos getBoardPos(Pos x, Pos y) const;
};

#endif
