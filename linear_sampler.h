#if !defined(LINEAR_SAMPLER_H)
#define LINEAR_SAMPLER_H

#include "sampler.h"

#include <cmath>

class linear_sampler : public sampler {
    public:
        unique_ptr<std::vector<vec3>> sample(int n) const {
            bool square_check = std::pow(std::round<int>(std::sqrt(n)), 2) == n;
            if (!square_check) {
                throw "Linear sampler got sample amount not perfect square.";
            }
            std::vector<vec3> samples = { vec3(0.0,0.0,0.0) };
            return make_unique<std::vector<vec3>>(samples);
        }
};

#endif // LINEAR_SAMPLER_H
