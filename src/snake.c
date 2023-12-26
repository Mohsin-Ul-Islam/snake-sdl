#include "snake.h"
#include <stdio.h>
#include <stdlib.h>

snake_t *snake_create(const size_t size, const snake_direction_t direction) {

  snake_t *snake = malloc(sizeof(snake_t));
  snake_piece_t *pieces = malloc(sizeof(snake_piece_t) * MAX_SNAKE_SIZE);

  for (size_t i = 0; i < size; i++) {
    pieces[i].x = 5;
    pieces[i].y = i + 3;
  }

  snake->size = size;
  snake->pieces = pieces;
  snake->direction = direction;
  snake->last_direction = direction;

  return snake;
}

void snake_destroy(snake_t *snake) {
  free(snake->pieces);
  free(snake);
  snake = NULL;
}

void snake_set_direction(snake_t *snake, const snake_direction_t direction) {
  // avoid setting the direction if the snake is not
  // ready to change the direction
  if (snake->last_direction != snake->direction) {
    return;
  }

  if ((snake->direction == UP && direction == DOWN) ||
      (snake->direction == DOWN && direction == UP) ||
      (snake->direction == LEFT && direction == RIGHT) ||
      (snake->direction == RIGHT && direction == LEFT)) {
    return;
  }

  snake->direction = direction;
}

void snake_eat(snake_t *snake) {
  // if the snake reached maximum size
  if (snake->size >= MAX_SNAKE_SIZE)
    return;

  // append a new snake piece after the tail
  snake->pieces[snake->size].x = snake->pieces[snake->size - 1].x;
  snake->pieces[snake->size].y = snake->pieces[snake->size - 1].y;

  snake->size += 1;

  return;
}

void snake_move(snake_t *snake) {
  int8_t dx = 0;
  int8_t dy = 0;

  switch (snake->direction) {
  case UP:
    dy = -1;
    break;
  case DOWN:
    dy = 1;
    break;
  case LEFT:
    dx = -1;
    break;
  case RIGHT:
    dx = 1;
    break;
  }

  // move the body from back
  for (size_t i = snake->size - 1; i >= 1; i--) {
    snake->pieces[i].x = snake->pieces[i - 1].x;
    snake->pieces[i].y = snake->pieces[i - 1].y;
  }

  // move the head along the new direction
  snake->pieces[0].x += dx;
  snake->pieces[0].y += dy;

  snake->last_direction = snake->direction;
}
