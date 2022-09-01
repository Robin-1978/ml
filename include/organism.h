#pragma once

#include <cmath>

#include "coords.h"
#include "ann.h"
#include "random.h"

namespace org
{

    template <typename T, T precision = 0.000001>
    bool Is(const T &a, const T &b)
    {
        auto v = a - b;
        return v < -precision || v > precision ? false : true;
    }

    struct Position : Pos
    {
        Position()
        {
            Random();
        }

        virtual void Random()
        {
            x = Random::Instance().RealInRange(-1.0, 1.0);
            y = Random::Instance().RealInRange(-1.0, 1.0);
        }

        bool operator==(const Position &p) const
        {
            auto dx = x - p.x;
            auto dy = y - p.y;
            if ((dx >= -precision && dx <= precision) || (dy >= -precision && dy <= precision))
            {
                return true;
            }
            return false;
        }
    };

    struct State : Position
    {
        double yaw, speed;

        void Step(double v , double w)
        {
            auto dx = speed * std::cos(yaw);
            auto dy = speed * std::sin(yaw);

            speed += v;

            if(speed < -1.0) speed = -1.0;
            if(speed > 1.0) speed = 1.0;

            yaw += w;
            if (yaw <= -M_PI)
                yaw += 2 * M_PI;
            if (yaw > M_PI)
                yaw -= 2 * M_PI;

            dxx =  speed * std::cos(yaw);
            dyy += speed * std::sin(yaw);
            x += (dx + dxx)/2;
            y += (dy + dyy)/2;
        }
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

        Polar Decide(const std::vector<Food> &foods, const std::vector<Organism> &organizations)
        {
            values v;
            for (auto &f : foods)
            {
                auto delta = f - *this;
                v.push_back(delta.x);
                v.push_back(delta.y);
            }
            for (auto p : organizations)
            {
                auto delta = p - *this;
                v.push_back(delta.x);
                v.push_back(delta.y);
            }
            auto decision = _brain(v);
            Step(decision[0], decision[1])
            // TODO:: Varify boundary
        }

    private:
        unsigned _score;
        org::Network _brain;
    };

    class World
    {
    public:
        World(unsigned oc, unsigned fc, double ratio = 1000)
            : _organisms(oc), _apples(fc)
        {
        }

        void Step()
        {
            for (auto &o : _organisms)
            {
                auto apples = GetApples(o);
                auto organizations = GetOrganisms(o);
                o.Decide(apples, organizations);
            }

            for (auto &o : _organisms)
            {
                o.yaw += o.steering / 4;
                if (p.yaw <= -M_PI)
                    p.yaw += 2 * M_PI;
                if (p.yaw > M_PI)
                    p.yaw -= 2 * M_PI;

                o.rho += o.speed;
                if (o.rho > 1.0)
                    o.rho -= 1.0;
                if (o.rho < -1.0)
                    o.rho = -1.0;

                o.theta += o.steering;
            }
        }

        std::vector<Food> GetApples(const Organism &o)
        {
        }

        std::vector<Food> GetOrganisms(const Organism &o)
        {
        }

    private:
        std::vector<Organism> _organisms;
        std::vector<Food> _apples;
    };

}