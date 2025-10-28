#include "sphere.h"
#include "box.h"

bool sphere::hit(const ray& r, interval ray_t, hit_record& rec) const {
    vec3 oc = center - r.origin();
    double a = r.direction().length_squared();
    double h = dot(r.direction(), oc);
    double c = oc.length_squared() - radius*radius;
    double discriminant = h*h - a*c;
    if (discriminant < 0) {
        return false;
    } 
    double sqrtd = std::sqrt(discriminant);
    double root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            return false;
        }
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    return true;
}

box sphere::aabb() const {
    return box(
        point3(center.x() - radius, center.y() - radius, center.z() - radius),
        point3(center.x() + radius, center.y() + radius, center.z() + radius)
    );
}