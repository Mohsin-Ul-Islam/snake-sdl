#include <stdlib.h>

#include "food.h"

food_t *food_create(const size_t x, const size_t y) {
  food_t *food = malloc(sizeof(food_t));

  food->x = x;
  food->y = y;

  return food;
}

void food_destroy(food_t *food) { free(food); }
