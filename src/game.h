#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

#include "food.h"
#include "snake.h"

#define PIECE_SIZE 20

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define SNAKE_INITIAL_SIZE 3

#define COLOR_RED 255, 0, 0
#define COLOR_BLUE 0, 0, 255
#define COLOR_GREEN 0, 255, 0
#define COLOR_BROWN 88, 57, 39

typedef struct game {
  bool is_over;
  size_t life_count;

  food_t *food;
  snake_t *snake;
  SDL_Rect *render_rect;
} game_t;

void game_tick();

void game_init();

void game_render(SDL_Renderer *renderer);

void game_handle_input(SDL_Event *event);

void game_destroy();

bool game_is_over();

#endif // GAME_H
