#ifndef CANH_BOARD_H
#define CANH_BOARD_H

#include "util.h"

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

        Cell() : m_state(HIDDEN), m_value(0) {}

    private:
        State m_state;
        Value m_value;

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
        : m_state(INIT),
        m_nRows(nRows),
        m_nCols(nCols),
        m_nMines(nMines),
        m_nHidden(nRows * nCols),
        m_lastPos(),
        m_cells(nRows * nCols)
    {
    }

    Cell::State getState(Pos p) const { return m_cells[p].m_state; }
    Cell::Value getValue(Pos p) const { return m_cells[p].m_value; }

    bool isWon() const { return m_state == WON; }
    bool isLost() const { return m_state == LOST; }

    Size getNRows() const { return m_nRows; }
    Size getNCols() const { return m_nCols; }
    Size getNMines() const { return m_nMines; }
    Size getNCells() const { return m_nRows * m_nCols; }

    Pos convertPos(Pos r, Pos c) const { return r * m_nCols + c; }
    Pos getLastOpenedPos() const { return m_lastPos; }
    std::vector<Pos> getNeighbors(Pos p) const;

    void open(Pos p);
    void nextState(Pos p);

private:
    State m_state;
    Size m_nRows;
    Size m_nCols;
    Size m_nMines;
    Size m_nHidden;

    Pos m_lastPos;

    std::vector<Cell> m_cells;

    void initCellValues(Pos safePos);
    void openRecur(Pos p);
};

#endif
