#include "graphic.h"
#include "board.h"
#include "util.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cstddef>
#include <string>

Graphic::Size Graphic::s_NIns = 0;

const std::string Graphic::SPRITE_PATH = "sprite.png";
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
bool insideRect(Graphic::Size x, Graphic::Size y, const SDL_Rect &rect);

Graphic::Graphic(const std::string &title, Size w, Size h)
    : m_Window(nullptr),
    m_Renderer(nullptr),
    m_SpriteTexture(nullptr),
    m_RedrawRequired(true),
    m_Board(nullptr)
{
    if (s_NIns == 0)
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

    m_Window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, w, h,
                                SDL_WINDOW_SHOWN);
    if (m_Window == nullptr)
    {
        throw Exception("Can not create window!");
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    if (m_Renderer == nullptr)
    {
        throw Exception("Can not create renderer!");
    }

    m_SpriteTexture = createTexture(m_Renderer, SPRITE_PATH);
    if (m_SpriteTexture == nullptr) {
        throw Exception("Can not create texture from \""
                        + SPRITE_PATH + "\"!");
    }

    s_NIns ++;
}

Graphic::~Graphic()
{
    s_NIns --;
    ASSERT(s_NIns >= 0);

    SDL_DestroyTexture(m_SpriteTexture);
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);

    if (s_NIns == 0)
    {

        IMG_Quit();
        SDL_Quit();
    }
}

SDL_Texture* createTexture(SDL_Renderer *renderer, const std::string &path)
{
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        throw Graphic::Exception("Cannot read image \"" + path + "\"!");
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Graphic::setBoard(Board *board, const SDL_Rect &boardRect)
{
    m_Board = board;
    m_BoardRect = boardRect;
    m_CellWidth = boardRect.w / board->getNCols();
    m_CellHeight = boardRect.h / board->getNRows();
    m_BoardSelecting = false;
    m_BoardLastPos = Board::POS_UNDEFINED;
}

void Graphic::draw()
{
    if (!m_RedrawRequired)
    {
        return;
    }
    m_RedrawRequired = false;

    SDL_RenderClear(m_Renderer);
    if (m_Board != nullptr)
    {
        drawBoard();
        if (m_BoardSelecting)
        {
            if (m_Board->getState(m_BoardLastPos) == Board::Cell::HIDDEN)
            {
                drawCell(m_BoardLastPos, SPRITE_RECTS[CELL_ZERO]);
            }
            else if (m_Board->getState(m_BoardLastPos) == Board::Cell::SHOWN)
            {
                drawCellNeighborsOpening(m_BoardLastPos);
            }
        }
    }
    SDL_RenderPresent(m_Renderer);
}

void Graphic::drawBoard()
{
    for (Board::Pos p = 0; p < m_Board->getNCells(); p ++)
    {
        drawCell(p, getSpriteRect(p));
    }
}

void Graphic::drawCell(Board::Pos p, const SDL_Rect &spriteRect)
{
    Board::Pos r = m_Board->getRow(p);
    Board::Pos c = m_Board->getCol(p);

    SDL_Rect destRect = {
        m_BoardRect.x + c * static_cast<Pos>(m_CellWidth),
        m_BoardRect.y + r * static_cast<Pos>(m_CellHeight),
        static_cast<Pos>(m_CellWidth),
        static_cast<Pos>(m_CellHeight)
    };

    SDL_RenderCopy(m_Renderer, m_SpriteTexture, &spriteRect, &destRect);
}

void Graphic::drawCellNeighborsOpening(Board::Pos pos)
{
    for (Board::Pos p : m_Board->getNeighbors(pos))
    {
        Board::Cell::State state = m_Board->getState(p);
        if (state == Board::Cell::SHOWN || state == Board::Cell::FLAGGED)
        {
            continue;
        }
        drawCell(p, SPRITE_RECTS[CELL_ZERO]);
    }
}

Board::Pos Graphic::getBoardPos(Graphic::Pos x, Graphic::Pos y)
{
    if (!insideRect(x, y, m_BoardRect))
    {
        return Board::POS_UNDEFINED;
    }

    Board::Pos r = (y - m_BoardRect.y) / m_CellHeight;
    Board::Pos c = (x - m_BoardRect.x) / m_CellWidth;

    return m_Board->convertPos(r, c);
}

SDL_Rect Graphic::getSpriteRect(Board::Pos p)
{
    Board::Cell::State cellState = m_Board->getState(p);
    Board::Cell::Value cellValue = m_Board->getValue(p);

    if (m_Board->isWon())
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
    else if (m_Board->isLost())
    {
        if (cellValue == Board::Cell::MINE)
        {
            if (p == m_BoardLastPos)
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

void Graphic::showError(const std::string &m)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Error", m.c_str(), nullptr);
}

bool insideRect(Graphic::Size x, Graphic::Size y, const SDL_Rect &rect)
{
    return rect.x <= x && x < rect.x + rect.w
           && rect.y <= y && y < rect.y + rect.h;
}

bool Graphic::handleEvent(const SDL_Event &e)
{
    switch(e.type)
    {
        case SDL_QUIT:
            return false;
        case SDL_MOUSEBUTTONDOWN:
            m_BoardLastPos = getBoardPos(e.button.x, e.button.y);
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                m_BoardSelecting = true;
                m_RedrawRequired = true;
            }
            else if (e.button.button == SDL_BUTTON_RIGHT)
            {
                m_Board->nextState(m_BoardLastPos);
                m_RedrawRequired = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                m_Board->open(m_BoardLastPos);
                m_BoardSelecting = false;
                m_RedrawRequired = true;
            }
            break;
        default:
            break;
    }
    return true;
}
