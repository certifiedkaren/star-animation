#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>

#define COUNT 1000

static SDL_FPoint pts[COUNT];
static float pts_speed[COUNT];

int main(int argc, char *argv[]) {

  SDL_Init(SDL_INIT_VIDEO);
  const int W = 600, H = 600;
  SDL_Window *window = SDL_CreateWindow("screen", W, H, 0);
  if (window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create window %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  SDL_SetRenderVSync(renderer, 1);
  int running = true;
  SDL_Event event;

  Uint64 last = SDL_GetTicks();
  srand(time(NULL));

  for (int i = 0; i < COUNT; i++) {
    int spawn_top = rand() % 2;
    int pos = rand() % W;
    pts_speed[i] = 30.0f + (rand() % 100);
    pts[i].x = spawn_top ? pos : 0.0f; 
    pts[i].y = !spawn_top ? pos : 0.0f;
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
        pts[i].x += pts_speed[i] * dt;
        pts[i].y += pts_speed[i] * dt;
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPoints(renderer, pts, SDL_arraysize(pts));

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
