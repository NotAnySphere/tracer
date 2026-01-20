#if !defined(CIRCLE_SAMPLER_H)
#define CIRCLE_SAMPLER_H

#include "../include/sampler.h"
#include "../include/rtweekend.h"

#include <cmath>

class circle_sampler : public sampler {
    public:
        void sample(std::vector<vec3> *samples) const override {
            
            double angle = (2*pi)/double(samples->size());

            for (size_t i = 0; i < samples->size(); i++)
            {
                samples->at(i) = 
                    vec3(std::cos(i*angle)/2, std::sin(i*angle)/2, 0.0);
            }
        }
};

#endif // CIRCLE_SAMPLER_H
