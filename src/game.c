#include <stdlib.h>

#include "game.h"

food_t *food;
snake_t *snake;
SDL_Rect *render_rect;

void render_food(SDL_Renderer *renderer, const food_t *food);
void render_snake(SDL_Renderer *renderer, const snake_t *snake);

void game_init() {
  snake = snake_create(SNAKE_INITIAL_SIZE, RIGHT);
  food = food_create(rand() % (SCREEN_WIDTH / PIECE_SIZE),
                     rand() % (SCREEN_HEIGHT / PIECE_SIZE));

  render_rect = malloc(sizeof(SDL_Rect));
  render_rect->w = PIECE_SIZE - 2;
  render_rect->h = PIECE_SIZE - 2;
}

void game_destroy() {
  food_destroy(food);
  snake_destroy(snake);
  free(render_rect);
  render_rect = NULL;
}

void game_render(SDL_Renderer *renderer) {
  render_food(renderer, food);
  render_snake(renderer, snake);
}

void game_tick() {
  // if game is not initialized just return
  if (snake == NULL || food == NULL)
    return;

  // first let the snake move
  snake_move(snake);

  // check if the snake eats the food
  snake_piece_t head = snake->pieces[0];
  if (head.x == food->x && head.y == food->y) {
    snake_eat(snake);

    food->x = rand() % (SCREEN_WIDTH / PIECE_SIZE);
    food->y = rand() % (SCREEN_HEIGHT / PIECE_SIZE);
  }
}

void game_handle_input(SDL_Event *event) {
  if (event->type == SDL_KEYDOWN) {
    switch (event->key.keysym.sym) {
    case SDLK_DOWN:
      snake_set_direction(snake, DOWN);
      break;
    case SDLK_UP:
      snake_set_direction(snake, UP);
      break;
    case SDLK_LEFT:
      snake_set_direction(snake, LEFT);
      break;
    case SDLK_RIGHT:
      snake_set_direction(snake, RIGHT);
      break;
    }
  }
}

void render_snake(SDL_Renderer *renderer, const snake_t *snake) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

  for (size_t i = 0; i < snake->size; i++) {
    if (i != 0) {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    }

    render_rect->x = snake->pieces[i].x * PIECE_SIZE;
    render_rect->y = snake->pieces[i].y * PIECE_SIZE;

    SDL_RenderFillRect(renderer, render_rect);
  }
}

void render_food(SDL_Renderer *renderer, const food_t *food) {
  render_rect->x = food->x * PIECE_SIZE;
  render_rect->y = food->y * PIECE_SIZE;

  SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, render_rect);
}
