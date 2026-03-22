#ifndef HITTABLE_H
#define HITTABLE_H

#include "../include/vec3.h"
#include "../include/ray.h"
#include "../include/interval.h"
#include "../include/tri.h"
#include "../include/box.h"
#include "../include/sphere.h"
#include "../include/aabb_bvh.h"

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
        enum {TRI, BOX, SPHERE, AABB} tag;
        union 
        {
            tri tri;
            box box;
            sphere sphere;
            aabb_bvh aabb_bvh;
        } uni;
        
        // Not sure how to make a clean initializer for this...

        ~hittable() = default;

        bool hit(const ray& r, interval ray_t, hit_record& rec) const {
            switch (tag)
            {
            case TRI:
                uni.tri.hit(r, ray_t, rec);
                break;
            case BOX:
                uni.box.hit(r, ray_t, rec);
                break;
            case SPHERE:
                uni.sphere.hit(r, ray_t, rec);
                break;
            case AABB:
                uni.aabb_bvh.hit(r, ray_t, rec);
                break;
            }
        }

        box aabb(void) const {
            switch (tag)
            {
            case TRI:
                return uni.tri.aabb();
                break;
            case BOX:
                return uni.box.aabb();
                break;
            case SPHERE:
                return uni.sphere.aabb();
                break;
            case AABB:
                return uni.aabb_bvh.aabb();
                break;
            }
        }

        void scale_by(const double factor) {
            switch (tag)
            {
            case TRI:
                uni.tri.scale_by(factor);
                break;
            case BOX:
                uni.box.scale_by(factor);
                break;
            case SPHERE:
                uni.sphere.scale_by(factor);
                break;
            case AABB:
                uni.aabb_bvh.scale_by(factor);
                break;
            }
        };
        
        void translate_by(const vec3& vec) {
            switch (tag)
            {
            case TRI:
                uni.tri.translate_by(vec);
                break;
            case BOX:
                uni.box.translate_by(vec);
                break;
            case SPHERE:
                uni.sphere.translate_by(vec);
                break;
            case AABB:
                uni.aabb_bvh.translate_by(vec);
                break;
            }
        };
        // virtual void rotate_by();
    
};

#endif