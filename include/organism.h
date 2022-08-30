#pragma once

#include <cmath>

#include "coords.h"
#include "ann.h"
#include "random.h"

namespace org
{

    struct Position : Polar
    {
        Position()
        {
            Random();
        }

        virtual void Random()
        {
            rho = Random::Instance().RealInRange(0.0, 1.0);
            theta = Random::Instance().RealInRange(M_PI, -M_PI);
        }
    };

    struct State : Polar
    {
        double yaw{}, speed{};
    };

    struct Food : Position
    {


    };

    class Organism : State
    {
    public:
    public:
        Organism()
            : _score{}, _brain{{{18, nullptr}, {36, std::make_shared<org::act::Tanh>()}, {2, std::make_shared<org::act::Tanh>()}}}
        {
        }

        Polar Decide(const std::vector<Food>& foods, const std::vector<Organism>& organizations)
        {
            values v;
            for(auto& f : foods)
            {
                auto delta = f - *this;
                v.push_back(delta.rho);
                v.push_back(delta.theta);
            }
            for(auto p : organizations)
            {
                auto delta = p -*this;
                v.push_back(delta.rho);
                v.push_back(delta.theta);
            }
        }
    private:
        unsigned _score;
        org::Network _brain;
    };

    class World
    {
    public:
        World(unsigned oc, unsigned fc)
            :_organisms(oc), _apples(fc)
        {

        }

        void Step()
        {
            for(auto& o : _organisms)
            {
                
            }
        }

        std::vector<Food> GetApples(const Organism& o)
        {
            
        }

        std::vector<Food> GetOrganism(const Organism& o)
        {

        }


    private:
        std::vector<Organism> _organisms;
        std::vector<Food> _apples;
    };

}