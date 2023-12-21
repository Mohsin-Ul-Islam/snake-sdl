#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

#include "game.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(void) {

  srand(time(NULL));

  int status = SDL_Init(SDL_INIT_VIDEO);
  if (status < 0) {
    SDL_Log("%s", SDL_GetError());
    return 1;
  }

  SDL_Event event;
  SDL_Window *window =
      SDL_CreateWindow("Snake", 0, 0, 640, 480, SDL_WINDOW_RESIZABLE);

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  game_init();
  uint64_t last = 0;

  bool done = false;
  while (!done) {

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
          done = true;
          break;
        }
      }

      game_handle_input(&event);
    }

    // ticks every half a second
    uint64_t current = SDL_GetTicks64();
    if ((current - last) >= 300) {
      last = current;
      game_tick();
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    game_render(renderer);
    SDL_RenderPresent(renderer);
  }

  game_destroy();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
