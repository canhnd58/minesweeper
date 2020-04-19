#ifndef CANH_BOARD_H
#define CANH_BOARD_H

#include "util.h"
#include "timer.h"

#include <cstdint>
#include <vector>

class Board
{
public:
    typedef int16_t Pos;
    typedef uint16_t Size;

    static const Pos POS_UNDEFINED = -1;

    class Cell
    {
    public:
        typedef uint8_t Value;
        static const Value MINE = 9;

        enum State
        {
            HIDDEN,
            FLAGGED,
            UNKNOWN,
            SHOWN
        };

        Cell() : m_State(HIDDEN), m_Value(0) {}

    private:
        State m_State;
        Value m_Value;

        friend class Board;
    };

    enum State
    {
        INIT,
        PLAYING,
        WON,
        LOST
    };

    Board(Size nRows, Size nCols, Size nMines)
        : m_State(INIT),
        m_NRows(nRows),
        m_NCols(nCols),
        m_NMines(nMines),
        m_NHidden(nRows * nCols),
        m_NFlagged(0),
        m_Timer(),
        m_Cells(nRows * nCols)
    {
    }

    Cell::State getState(Pos p) const { return m_Cells[p].m_State; }
    Cell::Value getValue(Pos p) const { return m_Cells[p].m_Value; }

    bool isWon() const { return m_State == WON; }
    bool isLost() const { return m_State == LOST; }

    Size getNRows() const { return m_NRows; }
    Size getNCols() const { return m_NCols; }
    Size getNMines() const { return m_NMines; }
    Size getNCells() const { return m_NRows * m_NCols; }

    Size getNMinesRemaining() const;

    Timer::Sec getElapsedSec() const { return m_Timer.getSecond(); }

    Pos convertPos(Pos r, Pos c) const { return r * m_NCols + c; }
    Pos getRow(Pos p) const { return p / m_NCols; }
    Pos getCol(Pos p) const { return p % m_NCols; }
    std::vector<Pos> getNeighbors(Pos p) const;

    void open(Pos p);
    void nextState(Pos p);

private:
    State m_State;
    Size m_NRows;
    Size m_NCols;
    Size m_NMines;
    Size m_NHidden;
    Size m_NFlagged;
    Timer m_Timer;

    std::vector<Cell> m_Cells;

    void initCellValues(Pos safePos);
    void openRecur(Pos p);
};

#endif
