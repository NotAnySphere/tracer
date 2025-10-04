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

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", 600, 250, 0, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }


    // World
    hittable_list world;

    // right, up, back
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0.5,1.5,-3), 0.4));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    
    std::array<point3, 3> verts = { point3(0,0,-1), point3(-1,1,-1), point3(-2,-1,-1) };
    world.add(make_shared<tri>(verts));

    // Camera
    auto cam = camera(16, 16.0/10.0, 1, make_unique<unit_sampler>());

    // Render

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
        SDL_RenderTexture(renderer, texture, NULL, NULL);
    }


    return 0;
}