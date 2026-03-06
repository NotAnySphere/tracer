#if !defined(UNIT_SAMPLER_H)
#define UNIT_SAMPLER_H

#include "../include/sampler.h"

class unit_sampler : public sampler {
    public:
        std::vector<vec3> sample_offsets(size_t samples) const override {
            auto offsets = std::vector<vec3>();
            offsets.resize(samples);
            offsets.at(0) = vec3(0, 0, 0);
            offsets.shrink_to_fit();
            return offsets;
        }
};

#endif // UNIT_SAMPLER_H
