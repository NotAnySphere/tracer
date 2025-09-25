#if !defined(LINEAR_SAMPLER_H)
#define LINEAR_SAMPLER_H

#include "sampler.h"

#include <cmath>

class linear_sampler : public sampler {
    public:
        void sample(std::vector<vec3> *samples) const override {
            
            int root = std::sqrt(samples->size());
            for (size_t i = 1; i < root + 1; i++)
            {
                for (size_t j = 1; j < root + 1; j++)
                {
                    samples->at(root*(i-1) + (j-1)) =
                        (vec3(double(i), double(j), 0.0) / (root+1)) - vec3(0.5,0.5,0.0);
                }
            }            
        }
};

#endif // LINEAR_SAMPLER_H
