#if !defined(SAMPLER_H)
#define SAMPLER_H

#include "../include/vec3.h"

#include <vector>

class sampler {
    public:
        virtual ~sampler() = default;

        // fills the vec with samples
        virtual void sample(std::vector<vec3>& samples) const {};
};

#endif // SAMPLER_H
