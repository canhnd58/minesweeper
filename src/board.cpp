#include "board.h"
#include "util.h"

#include <vector>
#include <algorithm>

std::vector<Board::Pos> Board::getNeighbors(Board::Pos p) const
{
    Pos r = p / m_nCols;
    Pos c = p % m_nCols;

    std::vector<Pos> pos;
    for (Pos i = -1; i <= 1; i ++)
    {
        for (Pos j = -1; j <= 1; j++)
        {
            Pos y = r + i;
            Pos x = c + j;
            if (0 <= y && y < m_nRows && 0 <= x && x < m_nCols)
            {
                pos.push_back(y * m_nCols + x);
            }
        }
    }
    return pos;
}

void Board::initCellValues(Board::Pos safeRow, Board::Pos safeCol)
{
    Pos safePos = safeRow * m_nCols + safeCol;

    std::vector<Size> mines(m_nRows * m_nCols);
    for (Size i = 0; i < mines.size(); i ++)
    {
        mines[i] = i;
    }
    std::shuffle(mines.begin(), mines.end(), Util::getRNG());

    Size i = 0, nMines = 0;
    while (nMines < m_nMines && i < mines.size())
    {
        if (mines[i] != safePos)
        {
            m_cells[mines[i]].m_value = Cell::MINE;
            nMines ++;
            for (Pos p : getNeighbors(mines[i]))
            {
                if (m_cells[p].m_value != Cell::MINE)
                {
                    m_cells[p].m_value ++;
                }
            }
        }
        i ++;
    }
    m_nMines = nMines;
}

void Board::open(Board::Pos r, Board::Pos c) {
    if (r == POS_UNDEFINED || c == POS_UNDEFINED)
    {
        return;
    }

    if (m_state == INIT)
    {
        initCellValues(r, c);
        m_state = PLAYING;
    }

    if (m_state != PLAYING)
    {
        return;
    }

    m_lastPos = PosPair(r, c);

    if (at(r, c).m_value == Cell::MINE)
    {
        m_state = LOST;
    }
    else
    {
        open(r * m_nCols + c);
        if (m_nHidden == m_nMines)
        {
            m_state = WON;
        }
    }
}

void Board::open(Board::Pos p)
{
    if (m_cells[p].m_state == Cell::SHOWN)
    {
        return;
    }

    m_cells[p].m_state = Cell::SHOWN;
    m_nHidden --;

    if (m_cells[p].m_value == 0)
    {
        for (Pos np : getNeighbors(p))
        {
            open(np);
        }
    }
}

Board::Cell& Board::at(Board::Pos r, Board::Pos c)
{
    ASSERT(r >= 0 && r < m_nRows);
    ASSERT(c >= 0 && c < m_nCols);
    return m_cells[r * m_nCols + c];
}

const Board::Cell& Board::at(Board::Pos r, Board::Pos c) const
{
    ASSERT(r >= 0 && r < m_nRows);
    ASSERT(c >= 0 && c < m_nCols);
    return m_cells[r * m_nCols + c];
}

void Board::nextState(Board::Pos r, Board::Pos c)
{
    if (r == POS_UNDEFINED || c == POS_UNDEFINED)
    {
        return;
    }

    switch (at(r, c).m_state)
    {
        case Cell::HIDDEN:
            at(r, c).m_state = Cell::FLAGGED;
            break;
        case Cell::FLAGGED:
            at(r, c).m_state = Cell::UNKNOWN;
            break;
        case Cell::UNKNOWN:
            at(r, c).m_state = Cell::HIDDEN;
            break;
        default:
            break;
    }
}
