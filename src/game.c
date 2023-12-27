#include <stdbool.h>
#include <stdlib.h>

#include "game.h"

game_t *game;

void render_food(SDL_Renderer *renderer);
void render_snake(SDL_Renderer *renderer);
void render_walls(SDL_Renderer *renderer);

void game_init() {
  game = malloc(sizeof(game_t));
  game->snake = snake_create(SNAKE_INITIAL_SIZE, RIGHT);

  // the -1 here compensates for the walls
  // we do not want to generate food on the walls
  game->food = food_create((rand() % ((SCREEN_WIDTH / PIECE_SIZE) - 1)),
                           rand() % ((SCREEN_HEIGHT / PIECE_SIZE) - 1));

  game->render_rect = malloc(sizeof(SDL_Rect));
  game->render_rect->w = PIECE_SIZE - 2;
  game->render_rect->h = PIECE_SIZE - 2;

  game->is_over = false;
  game->life_count = 3;
}

void game_destroy() {
  food_destroy(game->food);
  game->food = NULL;

  snake_destroy(game->snake);
  game->snake = NULL;

  free(game->render_rect);
  game->render_rect = NULL;

  free(game);
  game = NULL;
}

void game_render(SDL_Renderer *renderer) {
  render_food(renderer);
  render_snake(renderer);
  render_walls(renderer);
}

void game_tick() {
  // if game is not initialized just return
  if (game->snake == NULL || game->food == NULL)
    return;

  // check if the snake bumps into the top wall
  snake_piece_t head = game->snake->pieces[0];
  if (head.y == 0) {
    game->is_over = true;
    return;
  }

  // check if the snake bumps into the bottom wall
  if (head.y == SCREEN_HEIGHT / PIECE_SIZE) {
    game->is_over = true;
    return;
  }

  // check if the snake bumps into the left wall
  if (head.x == 0) {
    game->is_over = true;
    return;
  }

  // check if the snake bumps into the right wall
  if (head.x == SCREEN_WIDTH / PIECE_SIZE) {
    game->is_over = true;
    return;
  }

  // first let the snake move
  snake_move(game->snake);

  // check if the snake bumps into itself
  head = game->snake->pieces[0];
  for (size_t i = 1; i < game->snake->size; i++) {
    if (game->snake->pieces[i].x == head.x &&
        game->snake->pieces[i].y == head.y) {

      // chop the body
      game->snake->size = i;

      // decrement the life count
      game->life_count -= 1;
    }
  }

  // if no lives left
  if (game->life_count == 0) {
    game->is_over = true;
    return;
  }

  // check if the snake eats the food
  if (head.x == game->food->x && head.y == game->food->y) {
    snake_eat(game->snake);

    // generate food that is not on snake's body
    bool is_colliding = false;
    do {
      game->food->x = rand() % ((SCREEN_WIDTH / PIECE_SIZE) - 1);
      game->food->y = rand() % ((SCREEN_HEIGHT / PIECE_SIZE) - 1);

      is_colliding = false;
      for (size_t i = 0; i < game->snake->size; i++) {
        if (game->food->x == game->snake->pieces[i].x &&
            game->food->y == game->snake->pieces[i].y) {
          is_colliding = true;
          break;
        }
      }
    } while (is_colliding);
  }
}

void game_handle_input(SDL_Event *event) {
  if (event->type == SDL_KEYDOWN) {
    switch (event->key.keysym.sym) {
    case SDLK_DOWN:
      snake_set_direction(game->snake, DOWN);
      break;
    case SDLK_UP:
      snake_set_direction(game->snake, UP);
      break;
    case SDLK_LEFT:
      snake_set_direction(game->snake, LEFT);
      break;
    case SDLK_RIGHT:
      snake_set_direction(game->snake, RIGHT);
      break;
    }
  }
}

void render_snake(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, COLOR_RED, SDL_ALPHA_OPAQUE);

  for (size_t i = 0; i < game->snake->size; i++) {
    if (i != 0) {
      SDL_SetRenderDrawColor(renderer, COLOR_GREEN, SDL_ALPHA_OPAQUE);
    }

    game->render_rect->x = game->snake->pieces[i].x * PIECE_SIZE;
    game->render_rect->y = game->snake->pieces[i].y * PIECE_SIZE;

    SDL_RenderFillRect(renderer, game->render_rect);
  }
}

void render_food(SDL_Renderer *renderer) {
  game->render_rect->x = game->food->x * PIECE_SIZE;
  game->render_rect->y = game->food->y * PIECE_SIZE;

  SDL_SetRenderDrawColor(renderer, COLOR_BLUE, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, game->render_rect);
}

void render_walls(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, COLOR_BROWN, SDL_ALPHA_OPAQUE);

  // render top wall
  game->render_rect->y = 0;
  for (size_t i = 0; i < SCREEN_WIDTH / PIECE_SIZE; i++) {
    game->render_rect->x = i * PIECE_SIZE;
    SDL_RenderFillRect(renderer, game->render_rect);
  }

  // render bottom wall
  game->render_rect->y = SCREEN_HEIGHT - PIECE_SIZE;
  for (size_t i = 0; i < SCREEN_WIDTH / PIECE_SIZE; i++) {
    game->render_rect->x = i * PIECE_SIZE;
    SDL_RenderFillRect(renderer, game->render_rect);
  }

  // render left wall
  game->render_rect->x = 0;
  for (size_t i = 0; i < SCREEN_HEIGHT / PIECE_SIZE; i++) {
    game->render_rect->y = i * PIECE_SIZE;
    SDL_RenderFillRect(renderer, game->render_rect);
  }

  // render right wall
  game->render_rect->x = SCREEN_WIDTH - PIECE_SIZE;
  for (size_t i = 0; i < SCREEN_HEIGHT / PIECE_SIZE; i++) {
    game->render_rect->y = i * PIECE_SIZE;
    SDL_RenderFillRect(renderer, game->render_rect);
  }
}

bool game_is_over() { return game->is_over; }
