#if !defined(AABB_BVH)
#define AABB_BVH

#include "hittable.h"
#include "ray.h"

#include <memory>
#include <vector>

using std::shared_ptr;

class aabb_bvh : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        aabb_bvh() {}
        aabb_bvh(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        
        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }
    
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            bool hit = false;
            hit_record temp_rec;
            double closest_so_far = ray_t.max;

            for(const auto& object : objects) {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
                
            }
            return hit;
        }
};

#endif // AABB_BVH
