#if !defined(HITTABLE_LIST_H)
#define HITTABLE_LIST_H

#include "../include/hittable.h"
#include "../include/interval.h"
#include "../include/ray.h"
#include "../include/box.h"

#include <memory>
#include <vector>
#include <algorithm>

using std::shared_ptr;

class hittable_list : public hittable {
    public:
        std::vector<unique_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(unique_ptr<hittable> object) { add(std::move(object)); }

        void clear() { objects.clear(); }
        
        void add(unique_ptr<hittable> object) {
            objects.push_back(std::move(object));
        }
    
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
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

        void scale_by(double factor) override {
            for (auto &&i : objects)
            {
                i->scale_by(factor);
            }
        }

        void translate_by(vec3 vec) {
            for (auto &&i : objects)
            {
                i->translate_by(vec);
            }
        }
};

#endif // HITTABLE_LIST_H
