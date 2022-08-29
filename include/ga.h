#pragma once

#include <vector>
#include "dna.h"

namespace org
{
    struct Evaluation
    {
        std::vector<int> operator()(const std::vector<Dna> &dna)
        {
            return Eval(dna);
        }

        virtual std::vector<int> Eval(const std::vector<Dna> &dna)
        {
            return {};
        }
    };

    template <typename T>
    struct Ga
    {
        std::vector<Dna> dnas;
    };

}