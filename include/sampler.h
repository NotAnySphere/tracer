#if !defined(SAMPLER_H)
#define SAMPLER_H

#include "../include/vec3.h"

#include <vector>
#include <memory>

class sampler {
    public:
        virtual ~sampler() = default;

        virtual std::vector<vec3> sample_offsets(size_t samples) const = 0;
};

#endif // SAMPLER_H
