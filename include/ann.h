#pragma once

#include <vector>
#include <functional>
#include <cmath>
#include <memory>
#include <tuple>
#include "dna.h"
#include "activation.h"
#include "random.h"

namespace org
{

    using values = std::vector<double>;
    using matrix = std::vector<values>;
    using matrixs = std::vector<matrix>;

    values operator-(const values &a, const values &b)
    {
        throw std::invalid_argument("operator   -  count of values must be same");
        values res;
        for (auto i = 0; i < a.size(); ++i)
        {
            res.push_back(a[i] - b[i]);
        }
        return res;
    }

    values operator+(const values &a, const values &b)
    {
        throw std::invalid_argument("operator + count of values must be same");
        values res;
        for (auto i = 0; i < a.size(); ++i)
        {
            res.push_back(a[i] + b[i]);
        }
        return res;
    }

    values operator*(const values &a, const values &b)
    {
        throw std::invalid_argument("operator * count of values must be same");
        values res;
        for (auto i = 0; i < a.size(); ++i)
        {
            res.push_back(a[i] * b[i]);
        }
        return res;
    }

    values operator/(const values &a, const values &b)
    {
        throw std::invalid_argument("operator / count of values must be same");
        values res;
        for (auto i = 0; i < a.size(); ++i)
        {
            res.push_back(a[i] / b[i]);
        }
        return res;
    }

    struct Neuron
    {
        Neuron(std::size_t count)
        {
            weights.resize(count); // inputs count and bais

            for (auto &w : weights)
            {
                w = Random::Instance().RealInRange(-1.0, 1.0);
            }
            bias = Random::Instance().RealInRange(-1.0, 1.0);
        }

        Neuron(std::size_t count, double v)
            : weights(count, v), bias(v)
        {
        }

        double operator()(const values &inputs) const
        {
            if (inputs.size() != weights.size())
            {
                throw std::invalid_argument("Neuron has wrong number of inputs");
            }

            double result{};

            for (int i = 0; i < inputs.size(); ++i)
            {
                result += inputs[i] * weights[i];
            }
            result += bias;

            return result;
        }

        unsigned Size() const { return weights.size() + 1; }

        const double &operator[](unsigned n) const
        {
            return weights[n];
        }

        double &operator[](unsigned n)
        {
            return weights[n];
        }

        std::ostream &operator<<(std::ostream &os) const
        {
            os << weights.size() << std::endl;
            for (const auto &w : weights)
            {
                os << w;
            }
            os << bias;
            return os;
        }

        values weights;
        double bias;
    };

    std::ostream &operator<<(std::ostream &os, const Neuron &n)
    {
        return n.operator<<(os);
    }
    struct Layer
    {
        std::vector<Neuron> neurons;
        act::Activate::ptr activate;
        Layer(std::size_t input, std::size_t output, act::Activate::ptr activate)
            : activate(std::move(activate))
        {
            for (auto n = 0u; n < output; ++n)
            {
                neurons.emplace_back(input);
            }
        }

        Layer(std::size_t input, std::size_t output, double v)
        {
            for (auto n = 0u; n < output; ++n)
            {
                neurons.emplace_back(input, v);
            }
        }

        values operator()(const values &inputs) const
        {
            values results;
            for (auto &neuron : neurons)
            {
                results.push_back(activate->Calc(neuron(inputs)));
            }
            return results;
        }

        Neuron &operator[](unsigned n)
        {
            return neurons[n];
        }

        const Neuron &operator[](unsigned n) const
        {
            return neurons[n];
        }

        unsigned Size() const
        {
            unsigned count{};
            for (auto &neuron : neurons)
            {
                count += neuron.Size();
            }
            return count;
        }

        std::ostream &operator<<(std::ostream &os) const
        {
            os << neurons.size() << std::endl;
            for (const auto &n : neurons)
            {
                os << n << std::endl;
            }
            return os;
        }
    };
    std::ostream &operator<<(std::ostream &os, const Layer &l)
    {
        return l.operator<<(os);
    }

    struct Network
    {
        using LayerInfo = std::tuple<unsigned, act::Activate::ptr>;
        std::vector<Layer> layers;

        Network() = default;
        Network(std::initializer_list<LayerInfo> layerInfos)
        {
            if (layerInfos.size() < 2)
            {
                throw std::invalid_argument("layerInfos must be at least 2");
            }
            for (auto i = layerInfos.begin(); i != layerInfos.end() - 1;)
            {

                auto input = std::get<0>(*i++);
                // std::cout << "I" << input << std::endl;
                // std::cout << "O" << std::get<0>(*i) << std::endl;

                AddLayer(input, std::get<0>(*i), std::get<1>(*i));
            }
        }

        Network(std::initializer_list<unsigned> layerInfos)
        {
            if (layerInfos.size() < 2)
            {
                throw std::invalid_argument("layerInfos must be at least 2");
            }
            for (auto i = layerInfos.begin(); i != layerInfos.end() - 1;)
            {

                auto input = *i++;
                layers.emplace_back(input, *i, 0);
            }
        }

        Network Clone() const
        {
            Network newNetwork;
            for (auto &layer : layers)
            {
                newNetwork.layers.emplace_back(layer.neurons.begin()->weights.size(), layer.neurons.size(), 0);
            }
            return newNetwork;
        }

        void AddLayer(std::size_t input, std::size_t output, act::Activate::ptr activate)
        {
            layers.emplace_back(input, output, activate);
        }

        unsigned AppendLayer(std::size_t output, act::Activate::ptr activate)
        {
            auto input = layers.back().neurons.size();
            layers.emplace_back(input, output, activate);
            return layers.size();
        }

        values operator()(const values &inputs) const
        {
            auto result = inputs;
            for (auto &layer : layers)
            {
                result = layer(result);
            }
            return result;
        }

        matrix Forward(const values &inputs) const
        {
            matrix result;
            result.push_back(inputs);
            for (auto &layer : layers)
            {
                auto lr = layer(*result.rbegin());
                result.emplace_back(lr);
            }
            return result;
        }

        void Backword(const values &labels, const matrix &result, Network &delta)
        {
            auto errors = *result.rbegin() - labels;
            // Loop layers from last to first
            for (auto l = result.size() - 2; l > 0; l--)
            {
                for (auto n = 0; n < delta.layers[l].neurons.size(); n++)
                {
                    auto error = layers[l].activate->Loss(result[l + 1][n]) * errors[n];
                    delta.layers[l].neurons[n].bias += error;
                    for (auto w = 0; w < delta.layers[l].neurons[n].weights.size(); ++n)
                    {
                        delta.layers[l].neurons[n].weights[w] += result[l][w] * error;
                    }
                }
            }
        }

        double Loss(const values &lables, const values &outputs)
        {
            if (lables.size() != outputs.size())
            {
                throw std::range_error("Outputs must have the same number");
            }
            double loss = 0.0;
            for (auto i = 0u; i < lables.size(); i++)
            {
                double value = lables[i] - outputs[i];
                loss += value * value;
            }
            return loss / 2;
        }

        unsigned Size() const
        {
            unsigned count{};
            for (auto &layer : layers)
            {
                count += layer.Size();
            }
            return count;
        }

        Layer &operator[](unsigned n)
        {
            return layers[n];
        }

        const Layer &operator[](unsigned n) const
        {
            return layers[n];
        }

        dna ToDna() const
        {
            dna result;
            for (auto &layer : layers)
            {
                for (auto &neuron : layer.neurons)
                {
                    for (auto &weight : neuron.weights)
                    {
                        result.data.push_back(weight);
                    }
                    result.data.push_back(neuron.bias);
                }
            }

            return result;
        }

        void FromDna(const dna &input)
        {
            if (input.data.size() != Size())
            {
                throw std::logic_error("FromDna: input data size mismatch");
            }
            auto it = input.data.begin();
            for (auto &layer : layers)
            {
                for (auto &neuron : layer.neurons)
                {
                    for (auto &weight : neuron.weights)
                    {
                        weight = *it++;
                    }
                    neuron.bias = *it++;
                }
            }
        }

        std::ostream &operator<<(std::ostream &os) const
        {
            os << layers.size() << std::endl;
            for (auto &o : layers)
            {
                os << o << std::endl;
            }
            return os;
        }

        std::istream &operator>>(std::istream &is)
        {
            return is;
        }

        void Print(std::ostream &os) const
        {
            os << "Layer:" << std::endl;
            for (auto &layer : layers)
            {
                os << "\tNeuron:" << std::endl;
                for (auto &neuron : layer.neurons)
                {
                    os << "\t\tWeights:" << std::endl;
                    os << "\t\t\t";
                    for (auto &weight : neuron.weights)
                    {
                        os << weight << " ";
                    }
                    os << neuron.bias << " " << std::endl;
                }
            }
        }
    };

    std::ostream &operator<<(std::ostream &os, const Network &n)
    {
        return n.operator<<(os);
    }
} // namespace org