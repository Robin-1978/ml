#pragma once

#include <random>

namespace org
{

    class Random
    {
    public:
        static Random &Instance()
        {
            static Random instance;
            return instance;
        }

        // [ )
        template <typename T>
        T RealInRange(T minValue, T maxValue) 
        {
            std::uniform_real_distribution<> dist(minValue, maxValue);
            //
            return dist(_gen);
        }
        
        // [ ]
        template <typename T>
        T IntInRange(T minValue, T maxValue) 
        {
            std::uniform_int_distribution<> dist(minValue, maxValue);
            return dist(_gen);
        }

        //
        bool Hit(double probability) 
        {
            return RealInRange(0.0, 1.0) < probability ? true : false;
            return true;
        }

        //
        unsigned Roulette(const std::vector<unsigned>& fitness)
        {
            std::discrete_distribution<> dist(fitness.begin(), fitness.end());
            return dist(_gen);
        }

    private:
        Random()
            : _gen(std::random_device{}())
        {
        }

        std::mt19937 _gen;
    };

} // end of namespace org