#ifndef SNAKE_H
#define SNAKE_H

#include <stddef.h>

#define MAX_SNAKE_SIZE 100

// enum to represent the direction of a snake
typedef enum snake_direction {
  UP = 0,
  DOWN = 1,
  LEFT = 2,
  RIGHT = 3,
} snake_direction_t;

// smallest unit of the snake body
typedef struct snake_piece {
  // x coordinate of the snake piece
  size_t x;

  // y coordinate of the snake piece
  size_t y;
} snake_piece_t;

// structure of the snake including its head, body & direction
typedef struct snake {
  // pieces of the snake
  // pieces[0] will be the snake head
  snake_piece_t *pieces;

  // the current direction of the snake
  snake_direction_t direction;

  // the last direction the snake moved in
  snake_direction_t last_direction;

  // the current size of the snake
  size_t size;
} snake_t;

snake_t *snake_create(const size_t size, const snake_direction_t direction);

void snake_destroy(snake_t *snake);

void snake_eat(snake_t *snake);

void snake_move(snake_t *snake);

void snake_set_direction(snake_t *snake, const snake_direction_t direction);

#endif // SNAKE_H
