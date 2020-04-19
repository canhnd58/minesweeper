#include "board.h"
#include "graphic.h"
#include "util.h"

#include <SDL2/SDL.h>
#include <cstdint>
#include <cstdlib>

const uint32_t N_ROWS = 9;
const uint32_t N_COLS = 9;
const uint32_t N_MINES = 10;

const uint32_t WINDOW_WIDTH_UNIT = 32;
const uint32_t WINDOW_HEIGHT_UNIT = 32;


int main(int argc, char *argv[]) {
    Board board(N_ROWS, N_COLS, N_MINES);

    Graphic::Size windowWidth = N_COLS * WINDOW_WIDTH_UNIT;
    Graphic::Size windowHeight = N_ROWS * WINDOW_HEIGHT_UNIT;

    SDL_Rect boardRect = {
        0, 0,
        static_cast<int32_t>(windowWidth),
        static_cast<int32_t>(windowHeight)
    };

    try
    {
        Graphic gui("Minesweeper", windowWidth, windowHeight);
        gui.setBoard(&board, boardRect);

        bool quit = false;
        SDL_Event e;

        while (!quit)
        {
            gui.draw();
            while (SDL_PollEvent(&e) != 0)
            {
                if(!gui.handleEvent(e))
                {
                    quit = true;
                    break;
                }
            }
        }
    }
    catch (Graphic::Exception &e)
    {
        Graphic::showError(e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
