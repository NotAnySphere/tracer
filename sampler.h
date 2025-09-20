#if !defined(SAMPLER_H)
#define SAMPLER_H

#include "vec3.h"

#include <vector>
#include <memory>

using std::unique_ptr;
using std::make_unique;

class sampler {
    public:
        virtual ~sampler() = default;

        //returns an array with n sample offsets
        virtual unique_ptr<std::vector<vec3>> sample(int n) const {
            std::vector<vec3> samples = { vec3(0.0,0.0,0.0) };
            return make_unique<std::vector<vec3>>(samples);
        };
};

#endif // SAMPLER_H
