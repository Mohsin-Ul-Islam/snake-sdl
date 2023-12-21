#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

#include "food.h"
#include "snake.h"

#define SNAKE_INITIAL_SIZE 3

void game_tick();

void game_init();

void game_render(SDL_Renderer *renderer);

void game_handle_input(SDL_Event *event);

void game_destroy();

#endif // GAME_H
