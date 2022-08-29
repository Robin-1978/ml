#pragma once

#include <vector>
#include "random.h"

namespace org
{

    struct Dna
    {
        std::vector<double> data;

        Dna Corssover(const Dna &dna)
        {
            auto pos = Random::Instance().IntInRange(std::size_t(0), dna.data.size() - 1);
            Dna child;
            child.data.insert(child.data.end(), dna.data.begin(), dna.data.begin() + pos);
            child.data.insert(child.data.end(), data.begin() + pos, data.end());
            return child;
        }

        void Mutation(double probability)
        {
            if (Random::Instance().RealInRange(0.0, 1.0) < probability)
            {
                auto pos1 = Random::Instance().IntInRange(std::size_t(0u), data.size() - 1);
                auto pos2 = Random::Instance().IntInRange(std::size_t(0u), data.size() - 1);
                auto tmp = data[pos1];
                data[pos1] = data[pos2];
                data[pos2] = tmp;
            }
        }
    };

} // end org namespace