#pragma once

#include <vector>
#include "dna.h"

namespace org
{
    struct Evaluation
    {
        std::vector<int> operator()(const std::vector<dna> &dna)
        {
            return Eval(dna);
        }

        virtual std::vector<int> Eval(const std::vector<dna> &dna)
        {
            return {};
        }
    };

    struct Ga
    {
             
    };

}