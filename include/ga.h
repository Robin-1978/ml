#pragma once

#include <vector>
#include <tuple>
#include "dna.h"
#include "random.h"

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

    using Dnas = std::vector<std::tuple<dna, double>>;

    struct Ga
    {
        unsigned Select(const Dnas &dnas) const
        {
            auto slice = Random::Instance().RealInRange(0.0, 1.0);
            auto score = 0.0;
            for (int i = 0; i < dnas.size(); ++i)
            {
                score += std::get<1>(dnas[i]);
                if (score > slice)
                {
                    return i;
                }
            }
            return dnas.size() -1;
        }

        std::vector<dna> operator()(const Dnas &dnas, double percent) const
        {
            std::vector<dna> result;
            int count = std::ceil(dnas.size() * percent);
            for(int i=0; i < count; i++)
            {
                result.push_back(std::get<0>(dnas[i]));
            }
            for (auto i = 0; i < dnas.size() - count; ++i)
            {
                auto a = Select(dnas);
                auto b = Select(dnas);

                auto &ad = std::get<0>(dnas[a]);
                auto &bd = std::get<0>(dnas[b]);

                result.emplace_back(ad.Corssover(bd));
                result.rbegin()->Mutation(0.1, 0.2);
            }
            return result;
        }
    };

}