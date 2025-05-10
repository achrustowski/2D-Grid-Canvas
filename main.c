#include <stdio.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <math.h>

#define CELL_SIZE           36.0f
#define GRID_WIDTH          29
#define GRID_HEIGHT         23
#define SCREEN_WIDTH        (GRID_WIDTH * CELL_SIZE) + 1
#define SCREEN_HEIGHT       (GRID_HEIGHT * CELL_SIZE) + 1

bool is_running = true;
bool mouse_down = false;
bool clear_cells = false;
bool is_active[GRID_WIDTH * GRID_HEIGHT];

float mouse_x;
float mouse_y;

int grid_x[GRID_HEIGHT][GRID_WIDTH] = {{0}};;
int grid_y[GRID_HEIGHT][GRID_WIDTH] = {{0}};;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_FRect ghost_cell;
SDL_FRect active_cell;
SDL_FRect cells[GRID_WIDTH * GRID_HEIGHT];
SDL_FRect test_rect;

//deactivate all cells to clear all cells and empty the grid
void empty_grid()
{
    for (int i = 0; i < GRID_HEIGHT * GRID_WIDTH; ++i)
    {
        is_active[i] = false;
    }
}

//deactivate selected cells
void deactivate_cells(SDL_FRect* cells, float mouse_x, float mouse_y)
{
    for (int i = 0; i < GRID_HEIGHT * GRID_WIDTH; ++i)
    {
        if (mouse_x == cells[i].x && mouse_y == cells[i].y)
        {
            if (is_active[i] == true)
            {
                is_active[i] = false;
            }
        }
    }
}

//activate selected cells
void activate_cells(SDL_FRect* cells, float mouse_x, float mouse_y)
{
    for (int i = 0; i < GRID_HEIGHT * GRID_WIDTH; ++i)
    {
        if (mouse_x == cells[i].x && mouse_y == cells[i].y)
        {
            if (is_active[i] == false)
            {
                is_active[i] = true;
            }
        }
    }
}

//draw ghost cell to the screen
void draw_cells(SDL_FRect* ghost_cell)
{
    SDL_SetRenderDrawColor(renderer, 44, 44, 44, 255);
    SDL_RenderFillRect(renderer, ghost_cell);
}

//draw grid to the screen
void draw_grid(SDL_FRect* cells, SDL_FRect* test_rect)
{
    for (int i = 0; i < GRID_HEIGHT * GRID_WIDTH; ++i)
    {
        SDL_SetRenderDrawColor(renderer, 44, 44, 44, 255);
        SDL_RenderRect(renderer, &cells[i]);
        if (is_active[i] == true)
        {
            SDL_SetRenderDrawColor(renderer, 155, 155, 155, 255);
            SDL_RenderFillRect(renderer, &cells[i]);
        }
    }
}

//initialize 2d grid array with correct x and y values, then assign those values to cells array x and y members 
void init_cells(SDL_FRect* cells)
{
    int i, j, count1 = 0, count2 = 0;

    cells[0].x = 0;
    cells[0].y = 0;

    for (i = 0; i < GRID_HEIGHT * GRID_WIDTH; ++i)
    {
        cells[i].w = CELL_SIZE;
        cells[i].h = CELL_SIZE;
        is_active[i] = false;
    }

    for (i = 0; i < GRID_HEIGHT; ++i)
    {
        for (j = 1; j < GRID_WIDTH; ++j)
        {
            grid_x[i][j] += grid_x[i][j - 1] + CELL_SIZE; 
        }
    }

    for (i = 1; i < GRID_HEIGHT; ++i)
    {
        for (j = 0; j < GRID_WIDTH; ++j)
        {
            grid_y[i][j] += grid_y[i - 1][j] + CELL_SIZE;
        }
    }

    for (i = 0; i < GRID_HEIGHT; ++i)
    {
        for (j = 0; j < GRID_WIDTH; ++j)
        {
            cells[count1++].x = grid_x[i][j];
            cells[count2++].y = grid_y[i][j];
        }
    }

}

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Grid Canvas", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, NULL);

    SDL_HideCursor();

    init_cells(cells);
    empty_grid();

    ghost_cell.w = CELL_SIZE;
    ghost_cell.h = CELL_SIZE;

    while (is_running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    is_running = false;
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    ghost_cell.x = event.motion.x - fmodf(event.motion.x, CELL_SIZE);
                    ghost_cell.y = event.motion.y - fmodf(event.motion.y, CELL_SIZE);
                    if (mouse_down && !clear_cells)
                    {
                        activate_cells(cells, ghost_cell.x, ghost_cell.y);
                    }
                    if (mouse_down && clear_cells)
                    {
                        deactivate_cells(cells, ghost_cell.x, ghost_cell.y);
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    mouse_down = true;
                    mouse_x = event.motion.x - fmodf(event.motion.x, CELL_SIZE);
                    mouse_y = event.motion.y - fmodf(event.motion.y, CELL_SIZE);
                    if (mouse_down && !clear_cells)
                    {
                        activate_cells(cells, mouse_x, mouse_y);
                    }
                    if (mouse_down && clear_cells)
                    {
                        deactivate_cells(cells, mouse_x, mouse_y);
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    mouse_down = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.key)
                    {
                        case SDLK_C:
                            clear_cells = true;
                            break;
                        case SDLK_D:
                            clear_cells = false;
                            break;
                        case SDLK_E:
                            empty_grid();
                            break;
                    }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            draw_grid(cells, &test_rect);
            draw_cells(&ghost_cell);

            SDL_RenderPresent(renderer);
        }

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
