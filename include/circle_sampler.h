#if !defined(CIRCLE_SAMPLER_H)
#define CIRCLE_SAMPLER_H

#include "../include/sampler.h"
#include "../include/rtweekend.h"

#include <cmath>

class circle_sampler : public sampler {
    public:
        std::vector<vec3> sample_offsets(size_t samples) const override {
            
            auto offsets = std::vector<vec3>();
            offsets.resize(samples);
            
            double angle = (2*pi)/double(samples);

            for (size_t i = 0; i < samples; i++)
            {
                offsets[i] = vec3(std::cos(i*angle)/2, std::sin(i*angle)/2, 0.0);
            }
            return offsets;
        }
};

#endif // CIRCLE_SAMPLER_H
