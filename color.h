#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include "SDL3/SDL_surface.h"

using color = vec3;

void write_color(const color& pixel_color, int u, int v, SDL_Surface* surface) {

    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.0000, 0.9999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    auto color = SDL_MapSurfaceRGB(surface, rbyte, gbyte, bbyte);
    const auto pixels = (Uint32*)surface->pixels;
    pixels[(u) + (v * surface->w)] = color;
}

#endif