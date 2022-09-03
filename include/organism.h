#pragma once

#include <cmath>
#include <execution>

#include "coords.h"
#include "ann.h"
#include "random.h"
#include "ga.h"

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
    T Exceed(const T &v, const T &limit)
    {
        if (v < -limit)
            return v + 2 * limit;
        if (v > limit)
            return v - 2 * limit;
        return v;
    }

    template <typename T>
    T Limit(const T &v, const T &limit)
    {
        if (v < -limit)
            return -limit;
        if (v > limit)
            return limit;
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

            // std::cout <<"Speed:" << speed <<"POS:" << x <<","<< y <<"Yaw" << yaw <<std::endl;
        }
    };

    struct Food : Position
    {
        int step = 0;
    };

    struct Organism : public State
    {
        Organism()
            : _score{},
              _brain{{{6, nullptr},
                      {12, std::make_shared<org::act::Tanh>()},
                      {4, std::make_shared<org::act::Tanh>()},
                      {2, std::make_shared<org::act::Tanh>()}}}
        {
        }

        values Decide(const std::vector<Food> &foods, const std::vector<Organism> &organizations, double limit)
        {
            values v;
            for (auto &f : foods)
            {
                auto delta = f - *this;
                v.push_back(Exceed(delta.x, limit));
                v.push_back(Exceed(delta.y, limit));
                // std::cout << 2 << std::endl;
            }
            for (auto p : organizations)
            {
                auto delta = p - *this;
                v.push_back(Exceed(delta.x, limit));
                v.push_back(Exceed(delta.y, limit));
                v.push_back(p.speed);
                v.push_back(p.yaw);
                // std::cout << 4 << std::endl;
            }
            // std::cout << v.size() << std::endl;
            return _brain(v);
        }
        
        values Decide(const std::vector<Food> &foods, double limit)
        {
            values v;
            for (auto &f : foods)
            {
                auto delta = f - *this;
                v.push_back(Exceed(delta.x, limit));
                v.push_back(Exceed(delta.y, limit));
                // std::cout << 2 << std::endl;
            };
            return _brain(v);
        }
        unsigned _score;
        org::Network _brain;
    };

    struct World
    {
        World(unsigned oc, unsigned fc, double ratio = 500)
            : _organisms(oc), _apples(fc), _ratio(ratio), _lastScore{}, _curScore{}
        {
            for (auto &o : _organisms)
            {
                o.Random(ratio / 2);
            }
            for (auto &a : _apples)
            {
                a.Random(ratio / 2);
            }
        }

        void NextGeneration()
        {
            std::sort(_organisms.begin(), _organisms.end(), [this](const Organism &a, const Organism &b) -> bool
                      { return a._score > b._score; });
            _lastScore = _organisms[0]._score;
            std::cout << "Max Score: " << _lastScore << std::endl;
            double total{};

            for (auto &o : _organisms)
            {
                total += o._score;
            }

            Dnas dnas;
            for (auto &o : _organisms)
            {
                dnas.emplace_back(std::make_tuple(o._brain.ToDna(), double(o._score) / total));
            }
            auto ndnas = Ga()(dnas, 0.2);
            auto ndna = ndnas.begin();
            for (auto &o : _organisms)
            {
                o._brain.FromDna(*ndna++);
                o._score = 0;
            }
            _curScore = 0;
        }

        void Step()
        {
            Judge();
            Move();
        }

        void Move()
        {
            for (auto &o : _organisms)
            {
                auto apples = GetApples(o);
                //auto orgs = GetOrganisms(o);
                // std::cout << "----------------------------------------------------"   << std::endl;
                //  std::cout << apples.size() << std::endl;
                //  std::cout << orgs.size() << std::endl;
                //auto result = o.Decide({_apples[apples[0]], _apples[apples[1]], _apples[apples[2]]},
                //                       {_organisms[orgs[1]], _organisms[orgs[2]], _organisms[orgs[3]]});
                auto result = o.Decide({_apples[apples[0]], _apples[apples[1]], _apples[apples[2]]}, _ratio/2 );
                if (result[0] < 0)
                    result[0] = result[0] / 10;
                o.Step(result[0] / 100, result[1] / 30);
                o.x = Exceed(o.x, _ratio / 2);
                o.y = Exceed(o.y, _ratio / 2);
            }
        }

        void Judge()
        {
            std::for_each(std::execution::par, std::begin(_organisms), std::end(_organisms), [this](Organism &o)
            { 
                std::for_each(std::execution::par, std::begin(_apples), std::end(_apples), [&o, this](Food &a)
                {
                    if (a.step > 0)
                    {
                        a.step--;
                    }
                    else
                    {
                        auto dxa = Exceed(o.x - a.x, _ratio/2);
                        auto dya = Exceed(o.y - a.y, _ratio/2);
                        auto distance = dxa * dxa + dya * dya;
                        //std::cout << "Distance:" << std::sqrt(distance) << std::endl;
                        if (distance < 10 * 10)
                        {
                            o._score++;
                            a.step = 1000;
                            a.Random(_ratio / 2);

                            _curScore = std::max(_curScore, o._score);
                        }
                    } 
                 });
             });
        }

        std::vector<unsigned> GetApples(const Organism &o)
        {
            // std::cout << "****************************************************************" << std::endl;
            std::vector<unsigned> index;
            for (auto i = 0; i < _apples.size(); i++)
            {
                index.push_back(i);
            }
            std::sort(std::execution::par, index.begin(), index.end(), [&o, this](unsigned a, unsigned b)
            {
                if(_apples[a].step > 0) return false;
                if(_apples[b].step > 0) return true;
                auto dxa = Exceed(o.x - _apples[a].x, _ratio/2);
                auto dya = Exceed(o.y - _apples[a].y, _ratio/2);

                auto dxb = Exceed(o.x - _apples[b].x, _ratio/2);
                auto dyb = Exceed(o.y - _apples[b].y, _ratio/2);
                //std::cout << std::sqrt(dxa * dxa + dya * dya) << " " << std::sqrt(dxb * dxb + dyb * dyb) << std::endl;
                return (dxa * dxa + dya * dya) < (dxb * dxb + dyb * dyb); 
            });
            return index;
        }

        std::vector<unsigned> GetOrganisms(const Organism &o)
        {
            std::vector<unsigned> index;
            for (auto i = 0; i < _organisms.size(); i++)
            {
                index.push_back(i);
            }
            std::sort(std::execution::par, index.begin(), index.end(), [&o, this](unsigned a, unsigned b)
            {
                auto dxa = Exceed(o.x - _organisms[a].x, _ratio/2);
                auto dya = Exceed(o.y - _organisms[a].y, _ratio/2);

                auto dxb = Exceed(o.x - _organisms[b].x, _ratio/2);
                auto dyb = Exceed(o.y - _organisms[b].y, _ratio/2);
                return (dxa * dxa + dya * dya) < (dxb * dxb + dyb * dyb); 
            });
            return index;
        }

        std::vector<Organism> _organisms;
        std::vector<Food> _apples;
        double _ratio;
        unsigned _lastScore, _curScore;
    };

}