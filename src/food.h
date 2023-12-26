#ifndef FOOD_H
#define FOOD_H

#include <stddef.h>

typedef struct food {
  size_t x;
  size_t y;
} food_t;

food_t *food_create(const size_t x, const size_t y);

void food_destroy(food_t *food);

#endif // FOOD_H
