#if !defined(AABB_BVH)
#define AABB_BVH

#include "rtweekend.h"
#include "hittable.h"
#include "ray.h"
#include "box.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <optional>

using std::shared_ptr;
using std::optional;

class aabb_bvh : public hittable {
    public:
        unique_ptr<hittable> left;
        optional<unique_ptr<hittable>> right;
        box bb;

        aabb_bvh(std::vector<unique_ptr<hittable>>& objects, size_t start, size_t end) {
            
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
                left = std::move(objects[start]);
                right = {};
                bb = left->aabb();
            }
            else if (len == 2) 
            {
                left = std::move(objects[start]);
                right = std::move(objects[start + 1]);
                bb = box(left->aabb(), right.value()->aabb());
            } else {
                int size = int(double(len) / 2.0);
                left = make_unique<aabb_bvh>(objects, start, start + size);
                right = make_unique<aabb_bvh>(objects, start + size, end);
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
};

#endif // AABB_BVH
