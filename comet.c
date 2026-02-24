#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

// adds trails to the stars

#define COUNT 100
#define TAIL_LEN 200

typedef struct {
  float x, y;
  float vx, vy;
} Point;

typedef struct {
  Uint8 r, g, b, a;
} Line_Color;

typedef struct {
  float x1, y1;
  float x2, y2;
  int color_index;
} Line;

Point pts[COUNT];
Line line[COUNT];

Line_Color colors[] = {
    { 64, 156, 255, 255 },
    { 90, 180, 255, 255 },
    { 40, 120, 220, 255 },
    { 120, 200, 255, 255 },
    { 150, 230, 255, 255 },
    { 255, 255, 255, 255 },
    { 255, 244, 210, 255 },
    { 255, 230, 170, 255 },
    { 240, 240, 255, 255 },
    { 120, 255, 200, 255 },
    { 100, 255, 150, 255 },
    { 80, 220, 255, 255 },
};

int main(int argc, char *argv[]) {

  SDL_Init(SDL_INIT_VIDEO);
  const int W = 800, H = 800;
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

  int color_count = sizeof(colors) / sizeof(colors[0]);

  for (int i = 0; i < COUNT; i++) {
    int spawn_top = rand() % 2;
    int pos = rand() % W;
    float s = 30.0f + (rand() % 75);
    float x_val = spawn_top ? pos : 0.0f;
    float y_val = !spawn_top ? pos : 0.0f;
    pts[i].x = x_val;
    pts[i].y = y_val; 
    pts[i].vx = s;
    pts[i].vy = s;
    line[i].x1 = x_val;
    line[i].y1 = y_val;
    line[i].x2 = x_val;
    line[i].y2 = y_val;
    line[i].color_index = rand() % color_count;
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
        float x_val = spawn_top ? pos : 0.0f;
        float y_val = !spawn_top ? pos : 0.0f;
        pts[i].x = x_val;
        pts[i].y = y_val;
        line[i].x1 = x_val;
        line[i].y1 = y_val;
        line[i].x2 = x_val;
        line[i].y2 = y_val;
        line[i].color_index = rand() % color_count;
      } else {
        pts[i].x += pts[i].vx * dt;
        pts[i].y += pts[i].vy * dt;
        line[i].x2 = pts[i].x;
        line[i].y2 = pts[i].y;

        float dx = line[i].x2 - line[i].x1;
        float dy = line[i].y2 - line[i].y1;
        float len = hypotf(dx, dy);

        if (len >= TAIL_LEN) {
          float scale = TAIL_LEN / len;
          line[i].x1 = line[i].x2 - dx * scale;
          line[i].y1 = line[i].y2 - dy * scale;
        }
      }
    }

    for (int i = 0; i < color_count; i++) {
      SDL_SetRenderDrawColor(renderer, 
          colors[i].r, 
          colors[i].g, 
          colors[i].b, 
          colors[i].a);

      for (int j = 0; j < COUNT; j++) {
        if (line[j].color_index == i) {
          SDL_RenderLine(renderer, 
                       (int) line[j].x1, (int) line[j].y1, 
                       (int) line[j].x2, (int) line[j].y2);
        }
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
