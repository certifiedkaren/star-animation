#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>

#define COUNT 200
#define TAIL_LEN 30
#define RADIUS 50
#define PI 3.14159265

static SDL_FPoint pts_head[COUNT];
static SDL_FPoint pts_tail[COUNT];
static float pts_speed[COUNT];

int get_random_int(int min, int max) {
  return (rand() % (max - min + 1)) + min;
}

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

  for (int i = 0; i < COUNT; i++) {
    int center_x = W / 2;
    int center_y = H / 2;
    pts_speed[i] = 50.0f + (float)rand() / (float)RAND_MAX * 200.0f;
    float r = RADIUS;
    float angle = ((float)rand() / RAND_MAX) * 2.0f * PI;
    float x = center_x + r * cosf(angle);
    float y = center_y + r * sinf(angle);
    pts_head[i].x = x;
    pts_head[i].y = y;
    pts_tail[i].x = x;
    pts_tail[i].y = y;
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
      if (pts_head[i].x > W + 20 || pts_head[i].y > H + 20 || pts_head[i].x < -20 || pts_head[i].y < -20) {
        int center_x = W / 2;
        int center_y = H / 2;
        pts_speed[i] = 50.0f + (float)rand() / (float)RAND_MAX * 200.0f;

        float u1 = (float)rand() / RAND_MAX;
        float u2 = (float)rand() / RAND_MAX;
        float r = RADIUS;
        float angle = u2 * 2.0f * PI;
        float x = center_x + r * cosf(angle);
        float y = center_y + r * sinf(angle);
        pts_head[i].x = x;
        pts_head[i].y = y;
        pts_tail[i].x = x;
        pts_tail[i].y = y;
      }
      else {
        float dx_center = pts_head[i].x - W / 2.0f;
        float dy_center = pts_head[i].y - H / 2.0f;
        float len_center = SDL_sqrtf(dx_center*dx_center + dy_center*dy_center);
        if (len_center < 0.0001f) len_center = 0.0001f;
        float dirx = dx_center / len_center;
        float diry = dy_center / len_center;
        pts_head[i].x += dirx * pts_speed[i] * dt;
        pts_head[i].y += diry * pts_speed[i] * dt;
        pts_tail[i].x = pts_head[i].x - dirx * TAIL_LEN;
        pts_tail[i].y = pts_head[i].y - diry * TAIL_LEN;
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < COUNT; i++) {
      SDL_RenderLine(renderer, pts_head[i].x, pts_head[i].y, 
                     pts_tail[i].x, pts_tail[i].y);
    }
    SDL_RenderPoints(renderer, pts_head, COUNT);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
