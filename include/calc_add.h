#pragma once

#include <cmath>
#include <fstream>

#include "ann.h"

namespace org
{
namespace Calc
{
struct Object
{
    Object()
        :_score{}, 
        _brain{{
            {2, nullptr},
            {10, std::make_shared<org::act::Tanh>()},
            {1, std::make_shared<org::act::Tanh>()}
        }}
    {

    }

    double operator()(values& inputs)
    {
        values v;
        for(auto &i : inputs)
        {
            v.push_back((i - 50) / 50); // Normalization
        }
        auto result = _brain(v)[0];
        result = result * 5000 + 5000;
        _score += std::exp(2 - std::abs(inputs[0] * inputs[1] - result)/2500); // -4 ~ 2
        return result;
        
    }

    std::ostream& operator<<(std::ostream &os) const
    {
        _brain.ToDna().operator<<(os);

        return os;
    }

    std::istream& operator>>(std::istream &is)
    {
        dna dna;
        dna.operator>>(is);
        _brain.FromDna(dna);
        return is;
    }

    double _score;
    Network _brain;
};

class World
{
public:
    World(unsigned count=1000, unsigned step = 100, unsigned epoch = 50)
        :_objects(count), _step(step), _epoch(epoch)
    {
        std::ifstream ifs("calc.data", std::ios_base::binary);
        if(ifs)
        {
            this->operator>>(ifs);
        }
    }

    void NextGeneration()
    {
        std::sort(_objects.begin(), _objects.end(), [](Object &a, Object& b)
        {
            return a._score > b._score;
        });


        std::cout << std::endl;
        double total{};
        for(auto &o : _objects)
        {
            total += o._score;
            //std::cout << o._score << " ";
        }
        //total = total;

        std::cout <<"Max score: " << _objects[0]._score  << " Mean Score:" << total/_objects.size() << std::endl;


        Dnas dnas;
        for (auto &o : _objects)
        {
            dnas.emplace_back(std::make_tuple(o._brain.ToDna(), double(o._score) / total));
        }
        auto ndnas = Ga()(dnas, 0.1);
        auto ndna = ndnas.begin();
        for (auto &o : _objects)
        {
            //o._brain.Print(std::cout);
            o._brain.FromDna(*ndna++);
            //o._brain.Print(std::cout);
            o._score = 0;
        }
    }

    void Train()
    {

        for(auto e = 0; e < _epoch; ++e)
        {
            for(auto s =0; s < _step; ++s)
            {
                values v;
                v.push_back(Random::Instance().IntInRange(0, 100));
                v.push_back(Random::Instance().IntInRange(0, 100));
                std::for_each(std::execution::par, std::begin(_objects), std::end(_objects), [&v](Object& o){
                    o(v);
                });
            }
            std::cout << "Epo:" << e << std::endl;
            NextGeneration();
        }

        std::ofstream ofs("calc.data", std::ios_base::binary);
        if(ofs)
        {
            this->operator<<(ofs);
        }
    }

    double operator()(double a, double b)
    {
        values v;
        v.push_back(a);
        v.push_back(b);
        return _objects[0](v);
    }

    std::ostream& operator<<(std::ostream &os) const
    {

        std::size_t size = _objects.size();
        os.write(reinterpret_cast<char *>(&size), sizeof(size));

        for (const auto &o : _objects)
        {
            o.operator<<(os);
        }
        return os;

    }

    std::istream& operator>>(std::istream &is)
    {
        std::size_t size{};
        is.read(reinterpret_cast<char *>(&size), sizeof(size));
        _objects.resize(size);
        for (auto &o : _objects)
        {
            o.operator>>(is);
        }
        return is;

    }
private:
    std::vector<Object> _objects;
    unsigned _step, _epoch;
};

}}
