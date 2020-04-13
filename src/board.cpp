#include "board.h"
#include "util.h"

#include <vector>
#include <algorithm>

std::vector<Board::Pos> Board::getNeighbors(Board::Pos p) const
{
    std::vector<Pos> neighbors;
    if (p == POS_UNDEFINED)
    {
        return neighbors;
    }

    Pos r = p / m_nCols;
    Pos c = p % m_nCols;

    for (Pos i = -1; i <= 1; i ++)
    {
        for (Pos j = -1; j <= 1; j++)
        {
            Pos y = r + i;
            Pos x = c + j;
            if (0 <= y && y < m_nRows && 0 <= x && x < m_nCols)
            {
                neighbors.push_back(y * m_nCols + x);
            }
        }
    }
    return neighbors;
}

void Board::initCellValues(Board::Pos safePos)
{
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

void Board::open(Board::Pos p)
{
    if (p == POS_UNDEFINED || m_state == WON || m_state == LOST)
    {
        return;
    }

    if (m_state == INIT)
    {
        initCellValues(p);
        m_state = PLAYING;
    }

    m_lastPos = p;
    openRecur(p);

    if (m_state != LOST && m_nHidden == m_nMines)
    {
        m_state = WON;
    }
}

void Board::openRecur(Board::Pos p)
{
    if (m_cells[p].m_state != Cell::SHOWN)
    {
        m_cells[p].m_state = Cell::SHOWN;
        m_nHidden --;
    }

    if (m_cells[p].m_value == Cell::MINE)
    {
        m_state = LOST;
        return;
    }

    std::vector<Pos> neighbors = getNeighbors(p);
    Size nMineFound = 0;

    for (Pos np: neighbors)
    {
        if (m_cells[np].m_state == Cell::FLAGGED)
        {
            nMineFound ++;
        }
    }

    if (nMineFound >= m_cells[p].m_value)
    {
        for (Pos np : neighbors)
        {
            if (m_cells[np].m_state != Cell::SHOWN 
                && m_cells[np].m_state != Cell::FLAGGED)
            {
                openRecur(np);
            }
        }
    }
}

void Board::nextState(Board::Pos p)
{
    if (p == POS_UNDEFINED)
    {
        return;
    }

    switch (m_cells[p].m_state)
    {
        case Cell::HIDDEN:
            m_cells[p].m_state = Cell::FLAGGED;
            break;
        case Cell::FLAGGED:
            m_cells[p].m_state = Cell::UNKNOWN;
            break;
        case Cell::UNKNOWN:
            m_cells[p].m_state = Cell::HIDDEN;
            break;
        default:
            break;
    }
}
