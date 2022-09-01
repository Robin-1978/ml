#pragma once

#include <cmath>

#include "coords.h"
#include "ann.h"
#include "random.h"

namespace org
{

    constexpr double precision = 0.000001;

    template <typename T>
    bool Is(const T &a, const T &b)
    {
        auto v = a - b;
        return v < -precision || v > precision ? false : true;
    }

    template <typename T>
    T Exceed(const T& v, const T& limit)
    {
        if(v < -limit) return v + 2 * limit;
        if(v > limit) return v - 2 * limit;
        return v;
    }

    template <typename T>
    T Limit(const T& v, const T& limit)
    {
        if(v < -limit) return -limit;
        if(v > limit) return limit;
        return v;
    }



    struct Position : Pos
    {

        virtual void Random(double range)
        {
            x = Random::Instance().RealInRange(-range, range);
            y = Random::Instance().RealInRange(-range, range);
        }

        bool operator==(const Position &p) const
        {
            auto dx = x - p.x;
            auto dy = y - p.y;
            if (Is(dx, p.x) && Is(dy, p.y))
            {
                return true;
            }
            return false;
        }
    };

    struct State : Position
    {
        double yaw, speed;

        void Step(double v, double w)
        {
            auto dx = speed * std::cos(yaw);
            auto dy = speed * std::sin(yaw);

            speed += v;

            if (speed < -1.0)
                speed = -1.0;
            if (speed > 1.0)
                speed = 1.0;

            yaw += w;
            if (yaw <= -M_PI)
                yaw += 2 * M_PI;
            if (yaw > M_PI)
                yaw -= 2 * M_PI;

            auto dxx = speed * std::cos(yaw);
            auto dyy = speed * std::sin(yaw);
            x += (dx + dxx) / 2;
            y += (dy + dyy) / 2;
        }
    };

    struct Food : Position
    {
    };

    class Organism : public State
    {
    public:
    public:
        Organism()
            : _score{}, _brain{{{18, nullptr}, {36, std::make_shared<org::act::Tanh>()}, {2, std::make_shared<org::act::Tanh>()}}}
        {
        }

        values Decide(const std::vector<Food> &foods, const std::vector<Organism> &organizations)
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
            return _brain(v);
        }

    private:
        unsigned _score;
        org::Network _brain;
    };

    struct World
    {
        World(unsigned oc, unsigned fc, double ratio = 100)
            : _organisms(oc), _apples(fc), _ratio(ratio)
        {
            for(auto& o : _organisms)
            {
                o.Random(ratio);
            }   
            for(auto& a : _apples)
            {
                a.Random(ratio);
            }
        }

        void Step()
        {
            for (auto &o : _organisms)
            {
                auto apples = GetApples(o);
                auto organizations = GetOrganisms(o);
                auto result = o.Decide(apples, _organisms);
                o.Step(result[0], result[1]);
                // limit
            }
        }

        std::vector<Food> GetApples(const Organism &o)
        {
            std::sort(_apples.begin(), _apples.end(), [&o, this](const Food &a, const Food &b)->bool{
                auto dxa = Exceed(o.x - a.x, _ratio);
                auto dya = Exceed(o.y - a.y, _ratio);

                auto dxb = Exceed(o.x - a.x, _ratio);
                auto dyb = Exceed(o.y - a.y, _ratio);
                return (dxa * dxa + dya * dya) < (dxb * dxb + dyb * dxb);
            });
            return {_apples[0], _apples[1], _apples[2]};
        }

        std::vector<Organism> GetOrganisms(const Organism &o)
        {
            std::sort(_organisms.begin(), _organisms.end(), [&o, this](const Organism &a, const Organism &b)->bool{
                auto dxa = Exceed(o.x - a.x, _ratio);
                auto dya = Exceed(o.y - a.y, _ratio);

                auto dxb = Exceed(o.x - a.x, _ratio);
                auto dyb = Exceed(o.y - a.y, _ratio);
                return (dxa * dxa + dya * dya) < (dxb * dxb + dyb * dxb);
            });
            return {_organisms[0], _organisms[1], _organisms[2]};
        }

        std::vector<Organism> _organisms;
        std::vector<Food> _apples;
        double _ratio;
    };

}