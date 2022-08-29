#pragma once

#include <vector>
#include "random.h"
namespace org
{

    struct Dna
    {
        std::vector<double> data;

        Dna Corssover(const Dna &dna);
        {
            auto pos = Random::Instance().IntInRange(0, dna.data.size() - 1);
            Dna data;
            data.insert(0, dna.data.begin(), dna.data.begin() + pos);
            data.insert(0, data.begin() + pos, data.end());
            return data;
        }

        void Mutation(double probability)
        {
            if (Random::Instance().RealInRange(0.0, 1.0) < probability)
            {
                auto pos1 = Random::Instance().IntInRange(0, dna.data.size() - 1);
                auto pos2 = Random::Instance().IntInRange(0, dna.data.size() - 1);
                auto tmp = data[pos1];
                data[pos1] = data[pos2];
                data[pos2] = tmp;
            }
        }
    };

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