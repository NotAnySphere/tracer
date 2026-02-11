#if !defined(AABB_BVH)
#define AABB_BVH

#include "../include/rtweekend.h"
#include "../include/hittable.h"
#include "../include/ray.h"
#include "../include/box.h"

#include "../include/utils/arena.hpp"

#include <memory>
#include <vector>
#include <algorithm>
#include <optional>

using std::optional;

class aabb_bvh : public hittable {
    public:
        hittable* left;
        optional<hittable*> right;
        box bb;

        aabb_bvh(arena* alloc, std::vector<hittable*> objects, size_t start, size_t end) {

            //std::cout << "hi\n";

            int axis = int(random_double(0,2.99));

            auto comp = (axis == 0) ? box::compare_x
                      : (axis == 1) ? box::compare_y
                                    : box::compare_z;


            auto sort_start = objects.begin() + start;
            auto sort_end = objects.begin() + end;
            
            // sort hittables along that axis
            std::sort(sort_start, sort_end, comp);

            //std::cout << "sorted\n";

            size_t len = end - start;
            // split evenly into two groups
            
            if (len == 1)
            {
                left = objects[start];
                right = {};
                bb = left->aabb();
            }
            else if (len == 2) 
            {
                left = objects[start];
                right = objects[start + 1];
                bb = box(left->aabb(), right.value()->aabb());
            } else {
                int size = int(double(len) / 2.0);
                left = alloc->emplace_item<aabb_bvh>(alloc, objects, start, start + size);
                right = alloc->emplace_item<aabb_bvh>(alloc, objects, start + size, end);
                bb = box(left->aabb(), right.value()->aabb());
            }
        }
    
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            if (!bb.hit(r, ray_t, rec))
            {
                // std::cout << "missed!" << std::endl;
                return false;
            }
            
            // std::cout << "hit!" << std::endl;

            bool left_hit = false;
            bool right_hit = false;

            left_hit = left->hit(r, interval(ray_t.min, ray_t.max), rec);        
            
            if (right.has_value())
            {
                right_hit = right.value()->hit(r, interval(ray_t.min, left_hit ? rec.t : ray_t.max ), rec);
            }  
            return left_hit || right_hit;
        }

        box aabb() const override {
            return bb;
        }
        
        void scale_by(double factor) override {
            left->scale_by(factor);
            if (right.has_value()) right.value()->scale_by(factor);
            bb.scale_by(factor); 
        }

        void translate_by(vec3 vec) override {
            left->translate_by(vec);
            if (right.has_value()) right.value()->translate_by(vec);
            bb.translate_by(vec); 
        }
};

#endif // AABB_BVH
