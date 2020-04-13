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

    struct PosPair
    {
        Pos r, c;
        PosPair(Pos r, Pos c) : r(r), c(c) {}
        PosPair() : r(POS_UNDEFINED), c(POS_UNDEFINED) {}
    };

    class Cell
    {
    public:
        typedef uint16_t Value;
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
        m_cells(std::vector<Cell>(nRows * nCols))
    {
    }

    Cell::State getState(Pos r, Pos c) const
    {
        return const_cast<const Cell &>(at(r, c)).m_state;
    }
    Cell::Value getValue(Pos r, Pos c) const
    {
        return const_cast<const Cell &>(at(r, c)).m_value;
    }

    bool isWon() const { return m_state == WON; }
    bool isLost() const { return m_state == LOST; }

    Size getNRows() const { return m_nRows; }
    Size getNCols() const { return m_nCols; }
    Size getNMines() const { return m_nMines; }

    PosPair getLastOpenedPos() const { return m_lastPos; }

    void open(Pos r, Pos c);
    void nextState(Pos r, Pos c);

private:
    State m_state;
    Size m_nRows;
    Size m_nCols;
    Size m_nMines;
    Size m_nHidden;

    PosPair m_lastPos;

    std::vector<Cell> m_cells;

    std::vector<Pos> getNeighbors(Pos p) const;
    void initCellValues(Pos safeRow, Pos safeCol);

    void open(Pos p);

    Cell& at(Pos r, Pos c);
    const Cell& at(Pos r, Pos c) const;
};

#endif
