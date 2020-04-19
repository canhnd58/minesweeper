#include "board.h"
#include "graphic.h"

#include <cstdint>
#include <cstdlib>

const Board::Size N_ROWS = 9;
const Board::Size N_COLS = 9;
const Board::Size N_MINES = 10;

const Graphic::Size WINDOW_WIDTH_UNIT = 32;
const Graphic::Size WINDOW_HEIGHT_UNIT = 32;


int main(int argc, char *argv[]) {
    Graphic::Rect boardRect = {
        0,
        static_cast<Graphic::Pos>(4 * WINDOW_HEIGHT_UNIT),
        static_cast<Graphic::Pos>(N_COLS * WINDOW_WIDTH_UNIT),
        static_cast<Graphic::Pos>(N_ROWS * WINDOW_HEIGHT_UNIT)
    };

    Graphic::Rect bannerRect = {
        0, 0,
        boardRect.w,
        boardRect.y,
    };

    Graphic::Size windowWidth = boardRect.w;
    Graphic::Size windowHeight = bannerRect.h + boardRect.h;

    try
    {
        Graphic gui("Minesweeper", windowWidth, windowHeight);
        gui.createBoard(N_ROWS, N_COLS, N_MINES, boardRect);
        gui.createBanner(bannerRect);
        gui.loop();
    }
    catch (Graphic::Exception &e)
    {
        Graphic::showError(e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
