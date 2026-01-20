#pragma once

#include "../include/rtweekend.h"

class interval {
    public:
        double min, max;    

        interval() : min(+infinity), max(-infinity) {}

        interval(double min, double max) : min(min), max(max) {}

        static interval from(const double a, const double b) {
            if (a <= b) return interval(a, b);
            return interval(b, a);
        }

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
            if (other.max < inter.max) {
                inter.max = other.max;
            }
            return inter;
        }

        static const interval empty, universe;
};
