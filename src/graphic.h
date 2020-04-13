#ifndef CANH_GRAPHIC_H
#define CANH_GRAPHIC_H

#include "board.h"
#include "util.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <exception>
#include <string>

class Graphic
{
public:
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

        EMOJI_PRESSED,
        EMOJI_PLAYING,
        EMOJI_SELECTED,
        EMOJI_LOST,
        EMOJI_WON,

        SPRITE_TOTAL
    };

    class Exception : public std::runtime_error
    {
    public:
        Exception(const std::string &msg) : std::runtime_error(msg) {}
    };

    typedef uint32_t Size;
    typedef int32_t Pos;

    static void showError(const std::string &);

    Graphic(const std::string &title, Size w, Size h);
    ~Graphic();

    void setBoard(Board *board, const SDL_Rect &boardRect);
    Board::Pos getBoardPos(Pos x, Pos y);

    void draw();

private:
    static const std::string SPRITE_PATH;
    static std::vector<SDL_Rect> SPRITE_RECTS;

    static Size s_nIns;

    static void initSpriteRects();

    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_spriteTexture;

    Board *m_board;
    SDL_Rect m_boardRect;
    Size m_cellWidth;
    Size m_cellHeight;

    SDL_Rect getSpriteRect(Board::Pos p);
};

#endif
