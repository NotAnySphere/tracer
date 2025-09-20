#if !defined(LINEAR_SAMPLER_H)
#define LINEAR_SAMPLER_H

#include "sampler.h"

#include <cmath>

class linear_sampler : public sampler {
    public:
        unique_ptr<std::vector<vec3>> sample(int n) const override {
            
            std::vector<vec3> samples;
            int root = std::sqrt(n);
            for (size_t i = 1; i < root + 1; i++)
            {
                for (size_t j = 1; j < root + 1; j++)
                {
                    samples.push_back( 
                        (vec3(double(i), double(j), 0.0) / (root+1)) - vec3(0.5,0.5,0.0)
                    );
                }
            }
            
            return make_unique<std::vector<vec3>>(samples);
        }
};

#endif // LINEAR_SAMPLER_H
