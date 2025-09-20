#if !defined(CIRCLE_SAMPLER_H)
#define CIRCLE_SAMPLER_H

#include "sampler.h"
#include "rtweekend.h"

#include <cmath>

class circle_sampler : public sampler {
    public:
        unique_ptr<std::vector<vec3>> sample(int n) const override {
            
            std::vector<vec3> samples;
            double angle = (2*pi)/double(n);

            for (size_t i = 1; i < n + 1; i++)
            {
                samples.push_back( 
                    (vec3(std::cos(i*angle)/2, std::sin(i*angle)/2, 0.0))
                );
            
            }
            
            return make_unique<std::vector<vec3>>(samples);
        }
};

#endif // CIRCLE_SAMPLER_H
