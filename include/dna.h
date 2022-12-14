#pragma once

#include <vector>
#include <iostream>

#include "random.h"

namespace org
{

    template <typename T>
    struct basic_dna
    {
        std::vector<T> data;

        basic_dna Crossover(const basic_dna &dna) const
        {
            auto pos = Random::Instance().IntInRange(std::size_t(1), dna.data.size() - 2);
            basic_dna child;
            child.data.insert(child.data.end(), dna.data.begin(), dna.data.begin() + pos);
            child.data.insert(child.data.end(), data.begin() + pos, data.end());
            return child;
        }

        /**
         * @brief mutations of the  dna
         *
         * @param probability 0.0 to 1.0
         */
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

        void Mutation(double probability, double range)
        {
            if (Random::Instance().RealInRange(0.0, 1.0) < probability)
            {
                auto pos1 = Random::Instance().IntInRange(std::size_t(0u), data.size() - 1);
                //data[pos1] = data[pos1] + Random::Instance().RealInRange(-range, range);
                data[pos1] = Random::Instance().RealInRange(-1, 1);
                if(data[pos1] > 1) data[pos1] = 1;
                if(data[pos1] < -1) data[pos1] =-1;
            }
        }


        std::ostream &operator<<(std::ostream &os) const
        {
            std::size_t size = data.size();
            os.write(reinterpret_cast<char *>(&size), sizeof(size));
            for (const auto &n : data)
            {
                os.write(reinterpret_cast<const char *>(&n), sizeof(n));
            }
            return os;
        }

        std::istream &operator>>(std::istream &is)
        {
            std::size_t size{};
            is.read(reinterpret_cast<char *>(&size), sizeof(size));
            data.resize(size);

            for (auto n = 0u; n < size; ++n)
            {
                is.read(reinterpret_cast<char *>(&data[n]), sizeof(T));
            }

            return is;
        }
    };
    template <typename T>
    std::ostream &operator<<(std::ostream &os, const basic_dna<T>& d)
    {
        return d.operator<<(os);
    }

    template <typename T>
    std::istream &operator<<(std::istream &is, basic_dna<T>& d)
    {
        return d.operator>>(is);
    }
    
    using dna = basic_dna<double>;
    using fdna = basic_dna<float>;
} // end org namespace