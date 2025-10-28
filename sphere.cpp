#include "sphere.h"
#include "box.h"

box sphere::aabb() const {
    return box(
        point3(center.x() - radius, center.y() - radius, center.z() - radius),
        point3(center.x() + radius, center.y() + radius, center.z() + radius)
    );
}