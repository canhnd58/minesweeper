#include "graphic.h"
#include "board.h"
#include "util.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cstddef>
#include <string>


Graphic::Size Graphic::s_nIns = 0;
const std::string Graphic::SPRITE_PATH = "sprite.png";

const Graphic::Size Graphic::SPRITE_WIDTH_UNIT      = 16;
const Graphic::Size Graphic::SPRITE_HEIGHT_UNIT     = 16;

std::vector<SDL_Rect> Graphic::SPRITE_RECTS(Graphic::SPRITE_TOTAL);

void Graphic::initSpriteRects()
{
    SPRITE_RECTS[CELL_ZERO]           = {  0,  0, 16, 16};
    SPRITE_RECTS[CELL_ONE]            = { 16,  0, 16, 16};
    SPRITE_RECTS[CELL_TWO]            = { 32,  0, 16, 16};
    SPRITE_RECTS[CELL_THREE]          = { 48,  0, 16, 16};
    SPRITE_RECTS[CELL_FOUR]           = { 64,  0, 16, 16};
    SPRITE_RECTS[CELL_FIVE]           = { 80,  0, 16, 16};
    SPRITE_RECTS[CELL_SIX]            = { 96,  0, 16, 16};
    SPRITE_RECTS[CELL_SEVEN]          = {112,  0, 16, 16};
    SPRITE_RECTS[CELL_EIGHT]          = {128,  0, 16, 16};

    SPRITE_RECTS[CELL_MINE]           = {  0, 16, 16, 16};
    SPRITE_RECTS[CELL_MINE_WRONG]     = { 16, 16, 16, 16};
    SPRITE_RECTS[CELL_MINE_CURRENT]   = { 32, 16, 16, 16};
    SPRITE_RECTS[CELL_QUESTION_MARK]  = { 48, 16, 16, 16};
    SPRITE_RECTS[CELL_FLAG]           = { 64, 16, 16, 16};
    SPRITE_RECTS[CELL_UNOPENED]       = { 80, 16, 16, 16};

    SPRITE_RECTS[COUNT_ZERO]          = {  0, 32, 16, 32};
    SPRITE_RECTS[COUNT_ONE]           = { 16, 32, 16, 32};
    SPRITE_RECTS[COUNT_TWO]           = { 32, 32, 16, 32};
    SPRITE_RECTS[COUNT_THREE]         = { 48, 32, 16, 32};
    SPRITE_RECTS[COUNT_FOUR]          = { 64, 32, 16, 32};
    SPRITE_RECTS[COUNT_FIVE]          = { 80, 32, 16, 32};
    SPRITE_RECTS[COUNT_SIX]           = { 96, 32, 16, 32};
    SPRITE_RECTS[COUNT_SEVEN]         = {112, 32, 16, 32};
    SPRITE_RECTS[COUNT_EIGHT]         = {128, 32, 16, 32};
    SPRITE_RECTS[COUNT_NINE]          = {144, 32, 16, 32};

    SPRITE_RECTS[EMOJI_PRESSED]       = {  0, 64, 32, 32};
    SPRITE_RECTS[EMOJI_PLAYING]       = { 32, 64, 32, 32};
    SPRITE_RECTS[EMOJI_SELECTED]      = { 64, 64, 32, 32};
    SPRITE_RECTS[EMOJI_LOST]          = { 96, 64, 32, 32};
    SPRITE_RECTS[EMOJI_WON]           = {128, 64, 32, 32};
}


SDL_Texture* createTexture(SDL_Renderer *, const std::string &);

Graphic::Graphic(const std::string &title, Size w, Size h)
    : m_window(nullptr),
    m_renderer(nullptr),
    m_spriteTexture(nullptr)
{
    if (s_nIns == 0)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            throw Exception("Can not initialize SDL!");
        }

        
        IMG_InitFlags imgFlag = IMG_INIT_PNG;
        if ((IMG_Init(imgFlag) & imgFlag) != imgFlag)
        {
            throw Exception("Can not initialize SDL_image!");
        }
        initSpriteRects();
    }

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, w, h,
                                SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
    {
        throw Exception("Can not create window!");
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr)
    {
        throw Exception("Can not create renderer!");
    }

    m_spriteTexture = createTexture(m_renderer, SPRITE_PATH);
    if (m_spriteTexture == nullptr) {
        throw Exception("Can not create texture from \""
                        + SPRITE_PATH + "\"!");
    }

    s_nIns ++;
}

Graphic::~Graphic()
{
    s_nIns --;
    ASSERT(s_nIns >= 0);

    SDL_DestroyTexture(m_spriteTexture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    if (s_nIns == 0)
    {

        IMG_Quit();
        SDL_Quit();
    }
}

SDL_Texture* createTexture(SDL_Renderer *renderer, const std::string &path) {
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        throw Graphic::Exception("Cannot read image \"" + path + "\"!");
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Graphic::draw(const Board &board, const Util::Rect& rect)
{
    Board::Size nRows = board.getNRows();
    Board::Size nCols = board.getNCols();

    int32_t wdCellWidth = rect.w / nCols;
    int32_t wdCellHeight = rect.h / nRows;

    SDL_RenderClear(m_renderer);

    for (Board::Pos i = 0; i < nRows; i ++) {
        for (Board::Pos j = 0; j < nCols; j ++){
            SDL_Rect destRect = {
                rect.x + j * wdCellWidth,
                rect.y + i * wdCellHeight,
                wdCellWidth,
                wdCellHeight
            };

            SDL_Rect srcRect = getSpriteRect(board, i, j);
            SDL_RenderCopy(m_renderer, m_spriteTexture, &srcRect, &destRect);
        }
    }
    SDL_RenderPresent(m_renderer);

}
Board::PosPair Graphic::getBoardPos(const Board & board,
        const Util::Rect &rect, uint32_t x, uint32_t y)
{
    if (x < rect.x || x >= (rect.x + rect.w)
        || y < rect.y || y >= (rect.y + rect.h))
    {
        return Board::PosPair(Board::POS_UNDEFINED, Board::POS_UNDEFINED);
    }

    int32_t wdCellWidth = rect.w / board.getNCols();
    int32_t wdCellHeight = rect.h / board.getNRows();

    Board::Pos r = (y - rect.y) / wdCellHeight;
    Board::Pos c = (x - rect.x) / wdCellWidth;

    return Board::PosPair(r, c);
}

SDL_Rect Graphic::getSpriteRect(const Board &board,
    Board::Pos r, Board::Pos c)
{
    Board::Cell::State cellState = board.getState(r, c);
    Board::Cell::Value cellValue = board.getValue(r, c);
    
    if (board.isWon())
    {
        if (cellValue == Board::Cell::MINE)
        {
            return SPRITE_RECTS[CELL_FLAG];
        }
        else
        {
            ASSERT(0 <= cellValue && cellValue <= 8);
            return SPRITE_RECTS[CELL_ZERO + cellValue];
        }
    }
    else if (board.isLost())
    {
        if (cellValue == Board::Cell::MINE)
        {
            Board::PosPair lastPos = board.getLastOpenedPos();

            if (r == lastPos.r && c == lastPos.c)
            {
                return SPRITE_RECTS[CELL_MINE_CURRENT];
            }
            else
            {
                return SPRITE_RECTS[CELL_MINE];
            }
        }
        else
        {
            if (cellState == Board::Cell::HIDDEN)
            {
                return SPRITE_RECTS[CELL_UNOPENED];
            }
            else if (cellState == Board::Cell::FLAGGED)
            {
                return SPRITE_RECTS[CELL_MINE_WRONG];
            }
            else
            {
                ASSERT(0 <= cellValue && cellValue <= 8);
                return SPRITE_RECTS[CELL_ZERO + cellValue];
            }
        }
    }
    else
    {
        switch (cellState)
        {
            case Board::Cell::HIDDEN:
                return SPRITE_RECTS[CELL_UNOPENED];
            case Board::Cell::FLAGGED:
                return SPRITE_RECTS[CELL_FLAG];
            case Board::Cell::UNKNOWN:
                return SPRITE_RECTS[CELL_QUESTION_MARK];
            case Board::Cell::SHOWN:
                ASSERT(0 <= cellValue && cellValue <= 8);
                return SPRITE_RECTS[CELL_ZERO + cellValue];
        }
    }
}

void Graphic::showError(const std::string &m) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Error", m.c_str(), nullptr);
}
