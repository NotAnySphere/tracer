#if !defined(TRI_H)
#define TRI_H

#include "rtweekend.h"
#include "hittable.h"
#include "array"

#include <algorithm>

class tri : public hittable {
    public:
        explicit tri(const std::array<point3, 3> new_verts) : verts(new_verts) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            
            double eps = std::numeric_limits<double>::epsilon();

            vec3 e1 = verts[1] - verts[0];
            vec3 e2 = verts[2] - verts[0];
            
            vec3 cross_e1 = cross(r.direction(), e1);
            double det = dot(cross_e1, e2);

            if ( -eps < det && det < eps)
            {
                return false;
            }
            
            double inv_det = 1 / det;

            double u = inv_det * (dot( r.direction(), cross(r.origin() - verts[0], e2)));
            double v = inv_det * (dot( r.direction(), cross(e1, r.origin() - verts[0])));
            
            if (u <= 0 || 1 <= u)
            {
                return false;
            }
            
            if (v <= 0 || 1 <= v)
            {
                return false;
            }
            
            if (!(u + v <= 1))
            {
                return false;
            }
            
            double t = (-1) * inv_det * (dot(r.origin() - verts[0], cross(e1, e2)));

            if (!ray_t.surrounds(t)) {
                return false;
            }

            rec.t = t;
            rec.p = r.at(rec.t);
            const vec3 outward_normal = cross((e1),(e2));
            rec.set_face_normal(r, unit_vector(outward_normal));

            return true;
        }

        box aabb() const override {
            double x1 = verts[1].x();
            if (verts[0].x() < x1)
            {
                x1 = verts[0].x();
            }
            if (verts[2].x() < x1) {
                x1 = verts[2].x();
            }

            double y1 = verts[1].y();
            if (verts[0].y() < y1)
            {
                y1 = verts[0].y();
            }
            if (verts[2].y() < y1) {
                y1 = verts[2].y();
            }

            double z1 = verts[1].z();
            if (verts[0].z() < z1)
            {
                z1 = verts[0].z();
            }
            if (verts[2].z() < z1) {
                z1 = verts[2].z();
            }

            double x2 = verts[1].x();
            if (verts[0].x() < x2)
            {
                x2 = verts[0].x();
            }
            if (verts[2].x() < x2) {
                x2 = verts[2].x();
            }

            double y2 = verts[1].y();
            if (verts[0].y() < y2)
            {
                y2 = verts[0].y();
            }
            if (verts[2].y() < y2) {
                y2 = verts[2].y();
            }

            double z2 = verts[1].z();
            if (verts[0].z() < z2)
            {
                z2 = verts[0].z();
            }
            if (verts[2].z() < z2) {
                z2 = verts[2].z();
            }
            std::sort(verts.begin(), verts.end(),) // zoals hierboven of gwn een fatosenlijke sort, later handiger met polyhedra
            return box(point3(x1,y1,z1), point3(x2,y2,z2));
        }

    private:
        const std::array<point3, 3> verts;
};


#endif // TRI_H
