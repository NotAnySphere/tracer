#if !defined(AABB_BVH)
#define AABB_BVH

#include "hittable.h"
#include "ray.h"

#include <memory>
#include <vector>

using std::shared_ptr;

class aabb_bvh : public hittable {
    public:
        shared_ptr<aabb_bvh> left, right;

        aabb_bvh(std::vector<shared_ptr<hittable>> objects) { 
            // uuuh
            std::vector<shared_ptr<aabb_bvh>> boxes = ;

            this->left = aabb_bvh()
        }
    
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            bool hit = false;
            hit_record temp_rec;
            double closest_so_far = ray_t.max;

            if (left->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit = true;
                closest_so_far = temp_rec.t;
            }
            
            if (right->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit = true;
                return right->hit(r, ray_t, rec);
            }
                
            return left->hit(r, ray_t, rec);
        }
};

#endif // AABB_BVH
