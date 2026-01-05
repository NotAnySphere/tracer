#ifndef SPHERE_H
#define SPHERE_H

#include "rtweekend.h"
#include "hittable.h"

class sphere : public hittable {
    public:
        sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

        box aabb() const override;

        void scale_by(double factor) {
            center = center * factor;
            radius = radius * factor;
        }

        void translate_by(vec3 vec) {
            center = center + vec;
        }

    private:
        point3 center;
        double radius;
};


#endif
