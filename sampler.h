#if !defined(SAMPLER_H)
#define SAMPLER_H

#include "vec3.h"

#include <vector>
#include <memory>

using std::shared_ptr;
using std::unique_ptr;
using std::make_unique;

class sampler {
    public:
        virtual ~sampler() = default;

        // fills the vec with samples
        virtual void sample(std::vector<vec3> *samples) const {};
};

#endif // SAMPLER_H
