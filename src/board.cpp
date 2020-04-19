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

    Pos r = getRow(p);
    Pos c = getCol(p);

    for (Pos i = -1; i <= 1; i ++)
    {
        for (Pos j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            Pos y = r + i;
            Pos x = c + j;
            if (y < 0 || y >= m_NRows || x < 0 || x >= m_NCols)
            {
                continue;
            }
            neighbors.push_back(convertPos(y, x));
        }
    }
    return neighbors;
}

void Board::initCellValues(Board::Pos safePos)
{
    std::vector<Size> mines(m_NRows * m_NCols);
    for (Size i = 0; i < mines.size(); i ++)
    {
        mines[i] = i;
    }
    std::shuffle(mines.begin(), mines.end(), Util::getRNG());

    Size i = 0, nMines = 0;
    while (nMines < m_NMines && i < mines.size())
    {
        if (mines[i] != safePos)
        {
            m_Cells[mines[i]].m_Value = Cell::MINE;
            nMines ++;
            for (Pos p : getNeighbors(mines[i]))
            {
                if (m_Cells[p].m_Value != Cell::MINE)
                {
                    m_Cells[p].m_Value ++;
                }
            }
        }
        i ++;
    }
    m_NMines = nMines;
    m_Timer.start();
}

void Board::open(Board::Pos p)
{
    if (p == POS_UNDEFINED || m_State == WON || m_State == LOST
        || m_Cells[p].m_State == Cell::FLAGGED
        || m_Cells[p].m_State == Cell::UNKNOWN)
    {
        return;
    }


    if (m_State == INIT)
    {
        initCellValues(p);
        m_State = PLAYING;
    }

    openRecur(p);

    if (m_State != LOST && m_NHidden == m_NMines)
    {
        m_State = WON;
        m_Timer.stop();
    }
}

void Board::openRecur(Board::Pos p)
{
    if (m_Cells[p].m_State != Cell::SHOWN)
    {
        m_Cells[p].m_State = Cell::SHOWN;
        m_NHidden --;
    }

    if (m_Cells[p].m_Value == Cell::MINE)
    {
        m_State = LOST;
        m_Timer.stop();
        return;
    }

    std::vector<Pos> neighbors = getNeighbors(p);
    Size nMineFound = 0;

    for (Pos np: neighbors)
    {
        if (m_Cells[np].m_State == Cell::FLAGGED)
        {
            nMineFound ++;
        }
    }

    if (nMineFound >= m_Cells[p].m_Value)
    {
        for (Pos np : neighbors)
        {
            if (m_Cells[np].m_State != Cell::SHOWN
                && m_Cells[np].m_State != Cell::FLAGGED)
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

    switch (m_Cells[p].m_State)
    {
        case Cell::HIDDEN:
            m_Cells[p].m_State = Cell::FLAGGED;
            m_NFlagged ++;
            break;
        case Cell::FLAGGED:
            m_Cells[p].m_State = Cell::UNKNOWN;
            m_NFlagged --;
            break;
        case Cell::UNKNOWN:
            m_Cells[p].m_State = Cell::HIDDEN;
            break;
        default:
            break;
    }
}

Board::Size Board::getNMinesRemaining() const
{
    if (m_State == WON)
    {
        return 0;
    }
    return std::max(0, m_NMines - m_NFlagged);
}
