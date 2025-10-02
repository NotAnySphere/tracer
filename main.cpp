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

using std::unique_ptr;
using std::make_unique;

int main() {


    
    // World
    hittable_list world;

    // right, up, back
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0.5,1.5,-3), 0.4));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    
    std::array<point3, 3> verts = { point3(100,100,-1), point3(-100,100,-1), point3(0,-100,-1) };
    world.add(make_shared<tri>(verts));

    // Camera
    camera cam;
    
    cam.image_width = 400;
    cam.aspect_ratio = 16.0/10.0;
    cam.sampler_distribution = make_unique<unit_sampler>();
    cam.samples_per_pixel = 1;

    // Render

    cam.render(world);
    
    return 0;
}