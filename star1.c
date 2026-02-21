#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>

// gcc main.c -o app $(pkg-config --cflags --libs sdl3)
// implement pcg later for random nums

#define COUNT 500

typedef struct {
  float x, y;
  float vx, vy;
} point;

point pts[COUNT];

int main(int argc, char *argv[]) {

  SDL_Init(SDL_INIT_VIDEO);
  const int W = 600, H = 600;
  SDL_Window *window = SDL_CreateWindow("screen", W, H, 0);
  if (window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create window %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  int running = true;
  SDL_Event event;

  Uint64 last = SDL_GetTicks();
  srand(time(NULL));

  for (int i = 0; i < COUNT; i++) {
    int spawn_top = rand() % 2;
    int pos = rand() % W;
    float s = 30.0f + (rand() % 100);
    pts[i].x = spawn_top ? pos : 0.0f; 
    pts[i].y = !spawn_top ? pos : 0.0f;
    pts[i].vx = s;
    pts[i].vy = s;
  }

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT)
        running = false;
    }

    Uint64 now = SDL_GetTicks();
    float dt = (now - last) / 1000.0f;
    last = now;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < COUNT; i++) {
      if (pts[i].x > W || pts[i].y > H) {
        int spawn_top = rand() % 2;
        int pos = rand() % W;
        pts[i].x = spawn_top ? pos : 0.0f; 
        pts[i].y = !spawn_top ? pos : 0.0f;
      } else {
        pts[i].x += pts[i].vx * dt;
        pts[i].y += pts[i].vy * dt;
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < COUNT; i++) {
      SDL_RenderPoint(renderer, (int) pts[i].x, (int) pts[i].y);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
