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

    static const Size SPRITE_WIDTH_UNIT;
    static const Size SPRITE_HEIGHT_UNIT;

    static void showError(const std::string &);

    Graphic(const std::string &title, Size w, Size h);
    ~Graphic();

    void draw(const Board &, const Util::Rect &);
    Board::PosPair getBoardPos(const Board &, const Util::Rect &,
        uint32_t x, uint32_t y);

private:
    static Size s_nIns;
    static std::vector<SDL_Rect> SPRITE_RECTS;
    static const std::string SPRITE_PATH;
    static void initSpriteRects();

    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_spriteTexture;

    SDL_Rect getSpriteRect(const Board &, Board::Pos, Board::Pos);

};

#endif
