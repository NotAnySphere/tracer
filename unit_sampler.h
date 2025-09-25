#if !defined(UNIT_SAMPLER_H)
#define UNIT_SAMPLER_H

#include "sampler.h"

class unit_sampler : public sampler {
    public:
        void sample(std::vector<vec3> *samples) const override {
            samples->at(0) = vec3(0, 0, 0);
            samples->shrink_to_fit();
        }
};

#endif // UNIT_SAMPLER_H
