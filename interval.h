#if !defined(INTERVAl_H)
#define INTERVAl_H

#include "rtweekend.h"

class interval {
    public:
        double min, max;    

        interval() : min(+infinity), max(-infinity) {}

        interval(double min, double max) : min(min), max(max) {}

        double size() {
            return max - min;
        }

        double contains(double x) const {
            return min <= x && x <= max;
        }

        double surrounds(double x) const {
            return min < x && x < max;
        }

        double clamp(double x) const {
            if (x < min) return min;
            if (max < x) return max;
            return x;
        }

        interval intersection(interval other) const {
            interval inter = interval(min,max);
            if (inter.min < other.min) {
                inter.min = other.min;
            }
            if (inter.max > other.max) {
                inter.max = other.max;
            }
            return inter;
        }

        static const interval empty, universe;
};

const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif // INTERVAl_H
