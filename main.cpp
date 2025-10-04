#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "tri.h"
#include "camera.h"
#include "unit_sampler.h"
#include "linear_sampler.h"
#include "circle_sampler.h"

#include <memory>

//#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

using std::unique_ptr;
using std::make_unique;


int main() {
    // World
    hittable_list world;

    // right, up, back
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0.5,1.5,-3), 0.4));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    std::array<point3, 3> verts = { point3(0,0,-1), point3(-1,1,-1), point3(-2,-1,-1) };
    world.add(make_shared<tri>(verts));

    // Camera
    int WINDOW_WIDTH = 1920;
    double ASPECT_RATIO = 16.0 / 9.0;

    auto cam = camera(WINDOW_WIDTH, ASPECT_RATIO, 1, make_unique<unit_sampler>());

    // Render
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", WINDOW_WIDTH, cam.image_height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    auto surface = SDL_CreateSurface(cam.image_width, cam.image_height, SDL_PIXELFORMAT_RGBA32);

    SDL_LockSurface(surface);
    cam.render(world, surface);
    SDL_UnlockSurface(surface);

    auto texture = SDL_CreateTextureFromSurface(renderer, surface);

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        }
        SDL_RenderClear(renderer);
        //SDL_SetRenderDrawColor(renderer, 80, 0, 100, 255);
        //SDL_RenderFillRect(renderer, NULL);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }


    return 0;
}