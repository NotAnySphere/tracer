#if !defined(CAMERA_H)
#define CAMERA_H

#include "hittable.h"
#include "rtweekend.h"
#include "sampler.h"
#include "unit_sampler.h"

#include <vector>
#include <memory>

class camera {
    public:
        int image_width = 100;
        double aspect_ratio = 1.0;
        int samples_per_pixel = 1;
        unique_ptr<sampler> sampler_distribution;

        void render(const hittable& world) {
            initialize();
            
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int i = 0; i < image_height; i++) {
                std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
                

                std::vector<vec3> a;
                a.resize(samples_per_pixel);
                auto samples = std::make_unique<std::vector<vec3>>(a);
                
                for (int j = 0; j < image_width; j++) {
                    point3 pixel_center = pixel00_loc + (pixel_delta_u * j) + (pixel_delta_v * i);
                    color pixel_color = color(0.0,0.0,0.0);
                    
                    sampler_distribution->sample(samples.get());
                    for (size_t i = 0; i < samples->size(); i++)
                    {
                        vec3 offset = samples->at(i);
                        vec3 sample_direction = (pixel_center + (offset * pixel_delta_u.x())) - camera_center;
                        ray r = ray(camera_center, sample_direction);
                        pixel_color = pixel_color + (ray_color(r, world) / samples->size());
                    }

                    write_color(std::cout, pixel_color);
                }
            }
            std::clog << "\rDone.                 \n";
        }
    
    private:
        int image_height;
        point3 camera_center;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
    
        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;
        
            camera_center = point3(0, 0, 0);
            
            double focal_length = 1.0;
            double viewport_height = 2.0;
            double viewport_width = viewport_height * (double(image_width) / image_height);
                
            vec3 viewport_u = vec3(viewport_width, 0, 0);
            vec3 viewport_v = vec3(0, -viewport_height, 0);
                
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;
                
                
            point3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }
    
        color ray_color(const ray& r, const hittable& world) const {
            hit_record rec;
            if (world.hit(r, interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + color(1,1,1));
            }
        
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        }
};


#endif // CAMERA_H
