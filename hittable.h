#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.h"
#include "ray.h"
#include "interval.h"

class box;

class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;
        bool front_face;

        //assumed to have unit length
        void set_face_normal(const ray& r, const vec3& outward_normal) {

            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

        virtual box aabb() const = 0;

        virtual void scale_by(double factor) = 0;
        
        virtual void translate_by(vec3 vec) = 0;
        // virtual void rotate_by();
};

#endif