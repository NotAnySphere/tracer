#if !defined(AABB_BVH)
#define AABB_BVH

#include "rtweekend.h"
#include "hittable.h"
#include "ray.h"
#include "box.h"

#include <memory>
#include <vector>
#include <algorithm>

using std::shared_ptr;

class aabb_bvh : public hittable {
    public:
        shared_ptr<hittable> left, right;
        box bb;

        aabb_bvh(std::vector<shared_ptr<hittable>> objects, size_t start, size_t end) {
            // uuuh
            
            int axis = int(random_double(0,2.99));

            auto comp = (axis == 0) ? box::compare_x
                      : (axis == 1) ? box::compare_y
                                    : box::compare_z;


            auto sort_start = objects.begin() + start;
            auto sort_end = objects.begin() + end;
            
            // sort hittables along that axis
            std::sort(sort_start, sort_end, comp);

            size_t len = end - start;
            // split evenly into two groups
            
            if (len == 1)
            {
                left = objects[start];
                right = objects[start];
            }
            else if (len == 2) 
            {
                left = objects[start];
                right = objects[start + 1];
            } else {
                int size = int(double(len) / 2.0);
                left = make_shared<aabb_bvh>(objects, start, start + size);
                right = make_shared<aabb_bvh>(objects, start + size, end);
            }
            bb = box(left->aabb(), right->aabb());
        }
    
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            if (!bb.hit(r, ray_t, rec))
            {
                // std::cout << "missed!" << std::endl;
                return false;
            }
            
            // std::cout << "hit!" << std::endl;

            bool left_hit = left->hit(r, interval(ray_t.min, ray_t.max), rec);        
            bool right_hit = right->hit(r, interval(ray_t.min, left_hit ? rec.t : ray_t.max ), rec);
                
            return left_hit || right_hit;
        }

        box aabb() const override {
            return bb;
        }
};

#endif // AABB_BVH
