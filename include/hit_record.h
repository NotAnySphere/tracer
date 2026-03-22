#if !defined(HIT_RECORD_H)
#define HIT_RECORD_H

#include "interval.h"

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

#endif // HIT_RECORD_H
