#include "../include/rtweekend.h"
#include "../include/hittable.h"
#include "../include/hittable_list.h"
#include "../include/aabb_bvh.h"
#include "../include/sphere.h"
#include "../include/tri.h"
#include "../include/camera.h"
#include "../include/unit_sampler.h"
#include "../include/linear_sampler.h"
#include "../include/circle_sampler.h"
#include "../include/obj.h"

#include "../include/utils/thread_pool.hpp"
#include "../include/utils/arena.hpp"

#include <memory>
#include <sstream>
#include <chrono>

//#define SDL_MAIN_USE_CALLBACKS 1
#include <format>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

using std::unique_ptr;
using std::make_unique;


int main(int argv, char** args) {

    auto start = std::chrono::high_resolution_clock().now();

    bool write = false;
    if (argv > 1)
    {
        if (args[1][0] == 'w')
        {
            write = true;
        }
        
        std::cout << args[1] << std::endl;
    }
    
    auto obj1 = load("./models/bunny.obj");
    
    
    std::vector<unique_ptr<hittable>> bunnies = {};
    bunnies.resize(9);
    std::vector<unique_ptr<arena>> arenas = {};
    arenas.resize(9);
    auto pool = thread_pool(8);
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            auto task = [&](size_t i, size_t j) {
                arena
                std::cout << "reading bunny "<< (i * 3) + j << "\n";
                // arena alloc = arena(512);
                auto bunny = load("./models/bunny.obj");
                bunny.translate_by({((double)i) / 5.0,
                                    0,
                                    ((double)j) / 5.0});
                bunnies[(i * 3) + j] = make_unique<aabb_bvh>(bunny.bvh());
            };
            pool.enqueue(task, i, j);
        }
    }
    pool.join();
    // World
    aabb_bvh world = aabb_bvh(bunnies, 0, bunnies.size());
    // Camera
    // right, up, back
    int WINDOW_WIDTH = 1600;
    double ASPECT_RATIO = 16.0 / 10.0;

    auto cam = camera(WINDOW_WIDTH, ASPECT_RATIO, 1, make_unique<unit_sampler>());
    cam.translate(vec3(0, 0.3, 0.6));

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


    auto aabb_done = std::chrono::high_resolution_clock().now();
    std::cout << "AABB complete in: " << std::chrono::duration_cast<std::chrono::milliseconds>(aabb_done - start).count() << "ms" << std::endl;


    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                return 0;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                //cam.translate(vec3(-0.1, 0, 0));
                cam.translate(vec3(0, 0.05, 0));
                //cam.translate(vec3(-0.1, 0, 0));
            }
        }
        SDL_LockSurface(surface);
        cam.render(world, surface);
        SDL_UnlockSurface(surface);

        auto render_done = std::chrono::high_resolution_clock().now();
        std::cout << "render complete in: " << std::chrono::duration_cast<std::chrono::milliseconds>(render_done - aabb_done).count() << "ms" << std::endl;

        auto texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 80, 0, 100, 255);

        SDL_RenderTexture(renderer, texture, NULL, NULL);

        SDL_RenderDebugText(renderer, 5, 5, std::format("{} {} {}",
            cam.camera_center.x(),
            cam.camera_center.y(),
            cam.camera_center.z()).c_str());
        
        if (write)
        {
            SDL_SaveBMP(surface, "./build/image.bmp");
            return 0;
        }
        
        SDL_RenderPresent(renderer);
    }


    return 0;
}