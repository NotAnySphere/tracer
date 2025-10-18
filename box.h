#ifndef BOX_H
#define BOX_H

#include "rtweekend.h"
#include "hittable.h"

class box : public hittable {
    public:

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

    private:
        point3 p1, p2;
};


#endif
