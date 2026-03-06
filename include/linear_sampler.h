#if !defined(LINEAR_SAMPLER_H)
#define LINEAR_SAMPLER_H

#include "../include/sampler.h"

#include <cmath>

class linear_sampler : public sampler {
    public:
        std::vector<vec3> sample_offsets(size_t samples) const override {
            
            auto offsets = std::vector<vec3>();
            offsets.resize(samples);

            int root = std::sqrt(samples);
            for (size_t i = 1; i < root + 1; i++)
            {
                for (size_t j = 1; j < root + 1; j++)
                {
                    offsets.at(root*(i-1) + (j-1)) =
                        (vec3(double(i), double(j), 0.0) / (root+1)) - vec3(0.5,0.5,0.0);
                }
            }
            return offsets;
        }
};

#endif // LINEAR_SAMPLER_H
