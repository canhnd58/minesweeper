#include "board.h"
#include "graphic.h"
#include "util.h"

#include <SDL2/SDL.h>
#include <cstdint>

const uint32_t N_ROWS = 9;
const uint32_t N_COLS = 9;
const uint32_t N_MINES = 10;

const uint32_t WINDOW_WIDTH_UNIT = 32;
const uint32_t WINDOW_HEIGHT_UNIT = 32;

int main(int argc, char *argv[]) {
    Board board(N_ROWS, N_COLS, N_MINES);

    try
    {
        Graphic::Size windowWidth = N_COLS * WINDOW_WIDTH_UNIT;
        Graphic::Size windowHeight = N_ROWS * WINDOW_HEIGHT_UNIT;

        SDL_Rect boardRect = {
            0, 0,
            static_cast<int32_t>(windowWidth),
            static_cast<int32_t>(windowHeight)
        };

        Graphic gui("Minesweeper", windowWidth, windowHeight);
        gui.setBoard(&board, boardRect);

        bool quit = false;
        bool redrawRequired = true;
        SDL_Event e;

        while (!quit)
        {
            if (redrawRequired)
            {
                gui.draw();
                redrawRequired = false;
            }
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                    break;
                }
                else if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    SDL_MouseButtonEvent mouse = e.button;
                    Board::Pos p = gui.getBoardPos(mouse.x, mouse.y);

                    if (mouse.button == SDL_BUTTON_RIGHT)
                    {
                        board.nextState(p);
                        redrawRequired = true;
                    }
                    else if (mouse.button == SDL_BUTTON_LEFT)
                    {
                        board.open(p);
                        redrawRequired = true;
                    }
                }
            }
        }
    }
    catch (Graphic::Exception &e)
    {
        Graphic::showError(e.what());
    }
    return 0;
}
