#if !defined(HITTABLE_LIST_H)
#define HITTABLE_LIST_H

#include "hittable.h"
#include "ray.h"

#include <memory>
#include <vector>
#include <algorithm>

using std::shared_ptr;

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

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

        box aabb() const override {
            if (objects.size() < 1)
            {
                return box();
            }
            
            box aabb = objects[0]->aabb();
            for (size_t i = 0; i < objects.size(); i++)
            {
                aabb = box(aabb, objects[i]->aabb());
            }
            return aabb;
        }
};

#endif // HITTABLE_LIST_H
