#include <stdlib.h>

#include "game.h"

food_t *food;
snake_t *snake;

void render_food(SDL_Renderer *renderer, const food_t *food);
void render_snake(SDL_Renderer *renderer, const snake_t *snake);

void game_init() {
  food = food_create(rand() % 32, rand() % 24);
  snake = snake_create(SNAKE_INITIAL_SIZE, RIGHT);
}

void game_destroy() {
  food_destroy(food);
  snake_destroy(snake);
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

    food->x = rand() % 32;
    food->y = rand() % 24;
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

  SDL_Rect rect = (SDL_Rect){
      .w = SNAKE_PIECE_SIZE - 2,
      .h = SNAKE_PIECE_SIZE - 2,
  };

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
  for (size_t i = 0; i < snake->size; i++) {
    if (i != 0) {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    }
    rect.x = snake->pieces[i].x;
    rect.y = snake->pieces[i].y;

    SDL_RenderFillRect(renderer, &rect);
  }
}

void render_food(SDL_Renderer *renderer, const food_t *food) {

  SDL_Rect rect = (SDL_Rect){
      .w = FOOD_SIZE,
      .h = FOOD_SIZE,
      .x = food->x,
      .y = food->y,
  };

  SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &rect);
}
