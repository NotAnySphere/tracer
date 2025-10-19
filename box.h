#ifndef BOX_H
#define BOX_H

#include "rtweekend.h"
#include "hittable.h"

class box : public hittable {
    public:
        point3 p1, p2;

        box(const point3& a, const point3& b) {
            p1 = point3(a.x(),a.y(),a.z());
            p2 = point3(b.x(),b.y(),b.z());

            if (a.x() >= b.x()) {
                p1.e[0] = b.x();
                p2.e[0] = a.x();
            }
            if (a.y() >= b.y()) {
                p1.e[1] = b.y();
                p2.e[1] = a.y();
            }
            if (a.z() >= b.z()) {
                p1.e[2] = b.z();
                p2.e[2] = a.z();
            }
        }
        
        box(const box& a, const box& b) {
            p1 = a.p1;
            p2 = b.p2;
            // todo: dit juist
        }

        static bool compare(const shared_ptr<box> a, const shared_ptr<box> b, int axis) {
            return a->p1.e[axis] < b->p1.e[axis];
        }

        static bool compare_x(const shared_ptr<box> a, const shared_ptr<box> b) {
            return compare(a, b, 0);
        }
        static bool compare_y(const shared_ptr<box> a, const shared_ptr<box> b) {
            return compare(a, b, 0);
        }
        static bool compare_z(const shared_ptr<box> a, const shared_ptr<box> b) {
            return compare(a, b, 0);
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            interval x_t = interval((p1.x() - r.origin().x())/r.direction().x(),
                                    (p2.x() - r.origin().x())/r.direction().x());
            interval y_t = interval((p1.y() - r.origin().y())/r.direction().y(),
                                    (p2.y() - r.origin().y())/r.direction().y());
            interval z_t = interval((p1.z() - r.origin().z())/r.direction().z(),
                                    (p2.z() - r.origin().z())/r.direction().z());
            
            interval t = x_t.intersection(y_t).intersection(z_t);
                   
            if (t.size() < std::numeric_limits<double>::epsilon())
            {
                return false;
            }
            
            return true;
        }

        box aabb() const override {
            return box(p1,p2);
        }

    private:
};


#endif
