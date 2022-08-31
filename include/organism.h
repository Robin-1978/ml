#pragma once

#include <cmath>

#include "coords.h"
#include "ann.h"
#include "random.h"

namespace org
{

    template <typename T, T precision =  0.000001>
    bool Is(const T &a, const T &b)
    {
        auto v = a-b;
        return v < -precision  || v > precision ? false : true;
    }

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

        bool operator ==(const Position& p) constexpr
        {
            auto dRho = rho - p.rho;
            auto dTheta = theta - p.theta;
            if((dRho >= -precision && dRho <= precision) || (dTheta >= -precision && dTheta <= precision))
            {
                return true;
            }
            return false;
        }
    };

    struct State : Polar
    {
        double yaw;
        double steering, speed;
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

        void Step()
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
            auto decision = _brain(v);
            speed += decision[0];
            steering += decision[1];
            // TODO:: Varify boundary
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
                auto apples = GetApples(o);
                auto organizations = GetOrganizations(o);
                o.Decide(apples, organizations);
            }

            for(auto& o : _organisms)
            {
                if(o.steering < -2 * M_PI / 3)

                o.rho += o.speed;
                if(o.rho > 1.0) o.rho -= 1.0;
                if(o.rho < -1.0) o.rho = -1.0;

                o.theta += o.steering;

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