#pragma once

#include <vector>
#include <random>
#include <functional>
#include <cmath>


double tanh(double x)
{
    return std::tanh(x);
}

double sigmod(double x)
{
    return 1/(1+exp(-x));
}


using values = std::vector<double>;
using activation = std::function<double(double)>;

struct Neuron
{
    Neuron(const unsigned count)
    {
        std::random_device rd;
        std::default_random_engine e(rd());
        std::uniform_real_distribution<double> dbDist(-1.0, 1.0);
        weights.resize(count + 1);
        for(auto &w : weights)
        {
            w =  dbDist(e);
        }
    }

    double operator()(values inputs)  const
    {
        if(inputs.size() != weights.size() - 1)
        {
            std::__throw_out_of_range("");
        }

        double result{};
        for(int i = 0; i < inputs.size();++i)
        {
            result += inputs[i] * weights[i];
        }
        result += weights[inputs.size()];
        
        return result;
    }

    unsigned Size() const { return weights.size(); }

    values ToDna() const
    {
        return weights;
    }

    void FromDna(values::iterator& input)
    {
        for(auto &w : weights)
        {
            w = *input++;
        }
    }

    values weights;
};

struct Layer
{
    std::vector<Neuron> neurons;
    activation activate;
    Layer(const unsigned input, const unsigned output, activation activate)
    :activate(activate)
    {
        for(auto n = 0u; n < output; ++n)
        {
            neurons.emplace_back(input);
        }
    }

    values operator()(const values& inputs)  const
    {
        values results;
        for(auto &n : neurons)
        {
            results.push_back(activate(n(inputs)));
        }
        
        return results;
    }

    unsigned Size() const 
    {
        unsigned count{};
        for(auto& neuron : neurons)
        {
            count += neuron.Size();
        }
        return count;
    }

    values ToDna() const
    {
        values vs;
        for(auto &n : neurons)
        {
            vs.insert()
        }
    }
};

struct Network
{
std::vector<Layer> layers;

void AddInput(unsigned input, unsigned output, activation activate)
{
    layers.clear();
    layers.emplace_back(input, output, activate);
}

unsigned AddLayer(unsigned output, activation activate)
{
    auto input = layers.back().neurons.size();
    layers.emplace_back(input, output, activate);
    return layers.size();
}

values operator()(const values& inputs)
{
    auto result = inputs;
    for(auto& layer : layers)
    {
        result = layer(result);
    }
    return result;
}

unsigned Size() const 
{
    unsigned count{};
    for(auto& layer : layers)
    {
        count += layer.Size();
    }
    return count;
}

values ToDna() const
{
    values result;
    for(auto& layer : layers)
    {
        for(auto& neuron : layer.neurons)
        {
            for(auto& weight : neuron.weights)
            {
                result.push_back(weight);
            }
        }
    }
}

void FromDna(const values& values)
{

}


};