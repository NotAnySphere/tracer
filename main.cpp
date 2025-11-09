#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "aabb_bvh.h"
#include "sphere.h"
#include "tri.h"
#include "camera.h"
#include "unit_sampler.h"
#include "linear_sampler.h"
#include "circle_sampler.h"

#include <memory>

//#define SDL_MAIN_USE_CALLBACKS 1
#include <format>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

using std::unique_ptr;
using std::make_unique;


int main() {

    // World
    std::vector<shared_ptr<hittable>> hittables = {};
   
    // right, up, back
    //hittables.push_back(make_shared<sphere>(point3(0,0,-1), 0.5));
    //hittables.push_back(make_shared<sphere>(point3(0.5,1.5,-3), 0.4));
    //hittables.push_back(make_shared<sphere>(point3(0,-100.5,-1), 100));

    std::array<point3, 3> verts = { point3(0,0,-1), point3(-1,1,-1), point3(-2,-1,-1) };
    hittables.push_back(make_shared<tri>(verts));

    aabb_bvh world = aabb_bvh(hittables, 0, hittables.size() - 1);

    // Camera
    int WINDOW_WIDTH = 400;
    double ASPECT_RATIO = 16.0 / 10.0;

    auto cam = camera(WINDOW_WIDTH, ASPECT_RATIO, 1, make_unique<unit_sampler>());

    // Render
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", WINDOW_WIDTH, cam.image_height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    auto surface = SDL_CreateSurface(cam.image_width, cam.image_height, SDL_PIXELFORMAT_RGBA32);




    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                return 0;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                //cam.translate(vec3(-0.1, 0, 0));
                cam.translate(vec3(0, 0.1, 0));
                //cam.translate(vec3(-0.1, 0, 0));
            }
        }
        SDL_LockSurface(surface);
        cam.render(world, surface);
        SDL_UnlockSurface(surface);

        auto texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 80, 0, 100, 255);

        SDL_RenderTexture(renderer, texture, NULL, NULL);

        SDL_RenderDebugText(renderer, 5, 5, std::format("{} {} {}",
            cam.camera_center.x(),
            cam.camera_center.y(),
            cam.camera_center.z()).c_str());

        SDL_RenderPresent(renderer);
    }


    return 0;
}