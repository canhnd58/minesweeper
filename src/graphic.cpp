#include "graphic.h"
#include "board.h"
#include "util.h"
#include "timer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cstddef>
#include <string>
#include <memory>

const Graphic::Size Graphic::CELL_W     = 16;
const Graphic::Size Graphic::CELL_H     = 16;
const Graphic::Size Graphic::COUNT_W    = 13;
const Graphic::Size Graphic::COUNT_H    = 23;
const Graphic::Size Graphic::EMOJI_W    = 26;
const Graphic::Size Graphic::EMOJI_H    = 26;

Graphic::Size Graphic::s_NIns = 0;

const std::string Graphic::SPRITE_PATH = "sprite.png";
std::vector<SDL_Rect> Graphic::SPRITE_RECTS(Graphic::SPRITE_TOTAL);

void Graphic::initSpriteRects()
{
    SPRITE_RECTS[CELL_ZERO]     = {0*CELL_W,  0, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_ONE]      = {1*CELL_W,  0, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_TWO]      = {2*CELL_W,  0, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_THREE]    = {3*CELL_W,  0, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_FOUR]     = {4*CELL_W,  0, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_FIVE]     = {5*CELL_W,  0, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_SIX]      = {6*CELL_W,  0, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_SEVEN]    = {7*CELL_W,  0, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_EIGHT]    = {8*CELL_W,  0, CELL_W, CELL_H};

    SPRITE_RECTS[CELL_MINE]           = {0*CELL_W, CELL_H, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_MINE_WRONG]     = {1*CELL_W, CELL_H, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_MINE_CURRENT]   = {2*CELL_W, CELL_H, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_QUESTION_MARK]  = {3*CELL_W, CELL_H, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_FLAG]           = {4*CELL_W, CELL_H, CELL_W, CELL_H};
    SPRITE_RECTS[CELL_UNOPENED]       = {5*CELL_W, CELL_H, CELL_W, CELL_H};

    SPRITE_RECTS[COUNT_ZERO]    = {0*COUNT_W, 2*CELL_H, COUNT_W, COUNT_H};
    SPRITE_RECTS[COUNT_ONE]     = {1*COUNT_W, 2*CELL_H, COUNT_W, COUNT_H};
    SPRITE_RECTS[COUNT_TWO]     = {2*COUNT_W, 2*CELL_H, COUNT_W, COUNT_H};
    SPRITE_RECTS[COUNT_THREE]   = {3*COUNT_W, 2*CELL_H, COUNT_W, COUNT_H};
    SPRITE_RECTS[COUNT_FOUR]    = {4*COUNT_W, 2*CELL_H, COUNT_W, COUNT_H};
    SPRITE_RECTS[COUNT_FIVE]    = {5*COUNT_W, 2*CELL_H, COUNT_W, COUNT_H};
    SPRITE_RECTS[COUNT_SIX]     = {6*COUNT_W, 2*CELL_H, COUNT_W, COUNT_H};
    SPRITE_RECTS[COUNT_SEVEN]   = {7*COUNT_W, 2*CELL_H, COUNT_W, COUNT_H};
    SPRITE_RECTS[COUNT_EIGHT]   = {8*COUNT_W, 2*CELL_H, COUNT_W, COUNT_H};
    SPRITE_RECTS[COUNT_NINE]    = {9*COUNT_W, 2*CELL_H, COUNT_W, COUNT_H};

    Pos emojiH = 2 * CELL_H + COUNT_H;
    SPRITE_RECTS[EMOJI_SELECTING]     = {0*EMOJI_W, emojiH, EMOJI_W, EMOJI_H};
    SPRITE_RECTS[EMOJI_PLAYING]       = {1*EMOJI_W, emojiH, EMOJI_W, EMOJI_H};
    SPRITE_RECTS[EMOJI_CELL_SELECTING]= {2*EMOJI_W, emojiH, EMOJI_W, EMOJI_H};
    SPRITE_RECTS[EMOJI_LOST]          = {3*EMOJI_W, emojiH, EMOJI_W, EMOJI_H};
    SPRITE_RECTS[EMOJI_WON]           = {4*EMOJI_W, emojiH, EMOJI_W, EMOJI_H};
}


SDL_Texture* createTexture(SDL_Renderer *, const std::string &);
bool insideRect(Graphic::Size x, Graphic::Size y, const SDL_Rect &rect);

Graphic::Graphic(const std::string &title, Size w, Size h)
    : m_Window(nullptr),
    m_Renderer(nullptr),
    m_SpriteTexture(nullptr),
    m_RedrawRequired(true),
    m_ScaleW(1.0),
    m_ScaleH(1.0),
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

void Graphic::createBoard(Board::Size nRows, Board::Size nCols,
    Board::Size nMines, const SDL_Rect &boardRect)
{
    m_Board = std::make_unique<Board>(nRows, nCols, nMines);
    m_BoardRect = boardRect;
    m_BoardSelecting = false;
    m_BoardLastPos = Board::POS_UNDEFINED;

    m_ScaleW = boardRect.w / m_Board->getNCols() / CELL_W;
    m_ScaleH = boardRect.h / m_Board->getNRows() / CELL_H;
}

void Graphic::createBanner(const SDL_Rect &bannerRect)
{
    m_BannerRect = bannerRect;
    m_EmojiSelecting = false;

    Pos cX = m_BannerRect.x + m_BannerRect.w / 2;
    Pos cY = m_BannerRect.y + m_BannerRect.h / 2;

    m_EmojiRect = {
        cX - static_cast<Pos>(EMOJI_W * m_ScaleW / 2),
        cY - static_cast<Pos>(EMOJI_H * m_ScaleH / 2),
        static_cast<Pos>(EMOJI_W * m_ScaleW),
        static_cast<Pos>(EMOJI_H * m_ScaleH)
    };
}

void Graphic::loop()
{
    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        draw();
        while (SDL_PollEvent(&e) != 0)
        {
            if(!handleEvent(e))
            {
                quit = true;
                break;
            }
        }
    }
}

void Graphic::draw()
{
    Timer::Sec sec = m_Board->getElapsedSec();
    if (sec != m_LastDrawSec)
    {
        m_RedrawRequired = true;
    }
    if (!m_RedrawRequired)
    {
        return;
    }
    m_RedrawRequired = false;
    m_LastDrawSec = sec;

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
    drawEmoji();
    drawRemainingNMines();
    drawElapsedSec();
    SDL_RenderPresent(m_Renderer);
}

void Graphic::drawBoard() const
{
    for (Board::Pos p = 0; p < m_Board->getNCells(); p ++)
    {
        drawCell(p, getSpriteRect(p));
    }
}

void Graphic::drawCell(Board::Pos p, const SDL_Rect &spriteRect) const
{
    Board::Pos r = m_Board->getRow(p);
    Board::Pos c = m_Board->getCol(p);

    SDL_Rect destRect = {
        m_BoardRect.x + c * static_cast<Pos>(CELL_W * m_ScaleW),
        m_BoardRect.y + r * static_cast<Pos>(CELL_H * m_ScaleH),
        static_cast<Pos>(CELL_W * m_ScaleW),
        static_cast<Pos>(CELL_H * m_ScaleH)
    };

    SDL_RenderCopy(m_Renderer, m_SpriteTexture, &spriteRect, &destRect);
}

void Graphic::drawCellNeighborsOpening(Board::Pos pos) const
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

void Graphic::drawEmoji() const
{
    Sprite sprite = EMOJI_PLAYING;
    if (m_EmojiSelecting)
    {
        sprite = EMOJI_SELECTING;
    }
    else if (m_Board->isWon())
    {
        sprite = EMOJI_WON;
    }
    else if (m_Board->isLost())
    {
        sprite = EMOJI_LOST;
    }
    else if (m_BoardSelecting)
    {
        sprite = EMOJI_CELL_SELECTING;
    }

    SDL_RenderCopy(m_Renderer, m_SpriteTexture,
        &SPRITE_RECTS[sprite], &m_EmojiRect);
}

void Graphic::drawElapsedSec() const
{
    Timer::Sec sec = m_LastDrawSec > 999 ? 999 : m_LastDrawSec;

    Timer::Sec digits[] = {
        static_cast<Timer::Sec>(sec / 100),
        static_cast<Timer::Sec>((sec % 100) / 10),
        static_cast<Timer::Sec>(sec % 10)
    };

    Pos cY = m_BannerRect.y + m_BannerRect.h / 2;

    for (unsigned i = 0; i < 3; i ++)
    {
        SDL_Rect destRect = {
            m_BannerRect.x + m_BannerRect.w
                - static_cast<Pos>((3.5 - i) * COUNT_W * m_ScaleW),
            cY - static_cast<Pos>(COUNT_H * m_ScaleH / 2),
            static_cast<Pos>(COUNT_W * m_ScaleW),
            static_cast<Pos>(COUNT_H * m_ScaleH)
        };
        SDL_RenderCopy(m_Renderer, m_SpriteTexture,
            &SPRITE_RECTS[COUNT_ZERO + digits[i]], &destRect);
    }
}

void Graphic::drawRemainingNMines() const
{
    Board::Size nMines = m_Board->getNMinesRemaining();
    Board::Size digits[] = {
        static_cast<Board::Size>(nMines / 100),
        static_cast<Board::Size>((nMines % 100) / 10),
        static_cast<Board::Size>(nMines % 10)
    };

    Pos cY = m_BannerRect.y + m_BannerRect.h / 2;

    for (unsigned i = 0; i < 3; i ++)
    {
        SDL_Rect destRect = {
            m_BannerRect.x + static_cast<Pos>((i + 0.5) * COUNT_W * m_ScaleW),
            cY - static_cast<Pos>(COUNT_H * m_ScaleH / 2),
            static_cast<Pos>(COUNT_W * m_ScaleW),
            static_cast<Pos>(COUNT_H * m_ScaleH)
        };
        SDL_RenderCopy(m_Renderer, m_SpriteTexture,
            &SPRITE_RECTS[COUNT_ZERO + digits[i]], &destRect);
    }
}

Board::Pos Graphic::getBoardPos(Graphic::Pos x, Graphic::Pos y) const
{
    Board::Pos r = (y - m_BoardRect.y) / (CELL_W * m_ScaleW);
    Board::Pos c = (x - m_BoardRect.x) / (CELL_H * m_ScaleH);

    return m_Board->convertPos(r, c);
}

SDL_Rect Graphic::getSpriteRect(Board::Pos p) const
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

bool insideRect(Graphic::Pos x, Graphic::Pos y, const SDL_Rect &rect)
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
            if (insideRect(e.button.x, e.button.y, m_EmojiRect))
            {
                m_EmojiSelecting = true;
                m_RedrawRequired = true;
            }
            else if (insideRect(e.button.x, e.button.y, m_BoardRect))
            {
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
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                if (insideRect(e.button.x, e.button.y, m_EmojiRect))
                {
                    createBoard(m_Board->getNRows(), m_Board->getNCols(),
                        m_Board->getNMines(), m_BoardRect);
                }
                else if (insideRect(e.button.x, e.button.y, m_BoardRect)
                    && m_BoardLastPos == getBoardPos(e.button.x, e.button.y))
                {
                    m_Board->open(m_BoardLastPos);
                }
                m_EmojiSelecting = false;
                m_BoardSelecting = false;
                m_RedrawRequired = true;
            }
            break;
        default:
            break;
    }
    return true;
}
