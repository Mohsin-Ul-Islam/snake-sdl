#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_video.h>

#include "game.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void sdl_check(int status) {
  if (status < 0) {
    SDL_Log("%s", SDL_GetError());
    exit(1);
  }
}

void sdl_ptr_check(void *ptr) {
  if (ptr == NULL) {
    sdl_check(-1);
  }
}

int main(void) {

  srand(time(NULL));

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    SDL_Log("%s", SDL_GetError());
    return 1;
  }

  SDL_Event event;
  SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  sdl_ptr_check(window);

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  sdl_ptr_check(renderer);

  // initialize the sdl audio mixer
  sdl_check(Mix_OpenAudio(48000, AUDIO_S16SYS, 1, 2048));

  // load the game music file
  Mix_Chunk *music = Mix_LoadWAV("assets/audio/music.wav");
  sdl_ptr_check(music);

  // start playing music and loop indefinite
  sdl_check(Mix_PlayChannel(-1, music, 1));

  game_init();
  uint64_t last = 0;

  bool done = false;
  while (!done && !game_is_over()) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
          done = true;
          break;
        }
      }

      game_handle_input(&event);
    }

    // exit early if the game is done
    // avoiding an extra processing frame
    if (done) {
      break;
    }

    // ticks every 100 miliseconds
    uint64_t current = SDL_GetTicks64();
    if ((current - last) >= 100) {
      last = current;
      game_tick();
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    game_render(renderer);
    SDL_RenderPresent(renderer);
  }

  game_destroy();

  Mix_CloseAudio();
  Mix_FreeChunk(music);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
