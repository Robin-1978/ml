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

        virtual void Random(double range)
        {
            x = Random::Instance().RealInRange(-range, range);
            y = Random::Instance().RealInRange(-range, range);
            speed = Random::Instance().RealInRange(-1.0, 1.0);
            yaw = Random::Instance().RealInRange(-M_PI, M_PI);
        }

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

            //std::cout <<"Speed:" << speed <<"POS:" << x <<","<< y <<"Yaw" << yaw <<std::endl;
        }
    };

    struct Food : Position
    {
        int step = 0;
    };

    struct Organism : public State
    {
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
                //std::cout << 2 << std::endl;
            }
            for (auto p : organizations)
            {
                auto delta = p - *this;
                v.push_back(delta.x);
                v.push_back(delta.y);
                v.push_back(p.speed);
                v.push_back(p.yaw);
                //std::cout << 4 << std::endl;
            }
            //std::cout << v.size() << std::endl;
            return _brain(v);
        }

        unsigned _score;
        org::Network _brain;
    };

    struct World
    {
        World(unsigned oc, unsigned fc, double ratio = 500)
            : _organisms(oc), _apples(fc), _ratio(ratio)
        {
            for(auto& o : _organisms)
            {
                o.Random(ratio/2);
            }   
            for(auto& a : _apples)
            {
                a.Random(ratio/2);
            }
        }

        void Step()
        {
            for (auto &o : _organisms)
            {
                GetApples(o);
                GetOrganisms(o);

                for(auto &a : _apples)
                {
                    if(a.step > 0)
                    {
                        a.step--;
                        continue;
                    }

                    auto dxa = Exceed(o.x - a.x, _ratio);
                    auto dya = Exceed(o.y - a.y, _ratio);

                    if((dxa * dxa + dya * dya) < 5 * 5)
                    {
                        o._score++;
                        a.step = 1000;
                        a.Random(_ratio/2);
                    }
                }
                //std::cout << apples.size() << std::endl;
                //std::cout << orgs.size() << std::endl;
                auto result = o.Decide({_apples[0], _apples[1], _apples[2]}, {_organisms[0], _organisms[1], _organisms[2]});
                o.Step(result[0], result[1]/100);
                o.x = Exceed(o.x, _ratio/2);
                o.y = Exceed(o.y, _ratio/2);

            }
        }

        void GetApples(const Organism &o)
        {
            std::sort(_apples.begin(), _apples.end(), [&o, this](const Food &a, const Food &b)->bool{
                auto dxa = Exceed(o.x - a.x, _ratio);
                auto dya = Exceed(o.y - a.y, _ratio);

                auto dxb = Exceed(o.x - b.x, _ratio);
                auto dyb = Exceed(o.y - b.y, _ratio);
                return (dxa * dxa + dya * dya) < (dxb * dxb + dyb * dyb);
            });
            //return _apples;
        }

        void GetOrganisms(const Organism &o)
        {
            std::sort(_organisms.begin(), _organisms.end(), [&o, this](const Organism &a, const Organism &b)->bool{
                auto dxa = Exceed(o.x - a.x, _ratio);
                auto dya = Exceed(o.y - a.y, _ratio);

                auto dxb = Exceed(o.x - b.x, _ratio);
                auto dyb = Exceed(o.y - b.y, _ratio);
                return (dxa * dxa + dya * dya) < (dxb * dxb + dyb * dyb);
            });
           // return _organisms;
        }

        std::vector<Organism> _organisms;
        std::vector<Food> _apples;
        double _ratio;
    };

}