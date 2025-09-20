#if !defined(UNIT_SAMPLER_H)
#define UNIT_SAMPLER_H

#include "sampler.h"

class unit_sampler : public sampler {
    public:
        unique_ptr<std::vector<vec3>> sample(int n) const {
            std::vector<vec3> samples = { vec3(0.0,0.0,0.0) };
            return make_unique<std::vector<vec3>>(samples);
        }
};

#endif // UNIT_SAMPLER_H
