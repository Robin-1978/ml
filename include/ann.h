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

    struct Neuron
    {
        Neuron(const unsigned count)
        {
            weights.resize(count + 1);

            for (auto &w : weights)
            {
                w = Random::Instance().RealInRange(-1.0, 1.0);
            }
        }

        double operator()(values inputs) const
        {
            if (inputs.size() != weights.size() - 1)
            {
                throw std::invalid_argument("Neuron has wrong number of inputs");
            }

            double result{};
            for (int i = 0; i < inputs.size(); ++i)
            {
                result += inputs[i] * weights[i];
            }
            result += weights[inputs.size()];

            return result;
        }

        unsigned Size() const { return weights.size(); }

        values weights;
    };

    struct Layer
    {
        std::vector<Neuron> neurons;
        act::Activate::ptr activate;
        Layer(const unsigned input, const unsigned output, act::Activate::ptr activate)
            : activate(std::move(activate))
        {
            for (auto n = 0u; n < output; ++n)
            {
                neurons.emplace_back(input);
            }
        }

        values operator()(const values &inputs) const
        {
            values results;
            for (auto &n : neurons)
            {
                results.push_back(activate->Calc(n(inputs)));
            }

            return results;
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
    };

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

                AddLayer(input, std::get<0>(*i), std::get<1>(*i));
            }
        }

        void AddLayer(unsigned input, unsigned output, act::Activate::ptr activate)
        {
            layers.clear();
            layers.emplace_back(input, output, activate);
        }

        unsigned AppendLayer(unsigned output, act::Activate::ptr activate)
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
                auto layerResult = layer(*result.rbegin());
                result.emplace_back(layerResult);
            }
            return result;
        }

        void Backword(values &labels, matrix &result, matrix &deltas)
        {
            for (auto n = 0u; n < labels.size(); n++)
            {
                auto delta = (labels[n] - result.rbegin()->at(n)) * layers.rbegin()->activate->Loss(result.rbegin()->at(n));
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
                }
            }
        }

        std::ostream &operator<<(std::ostream &os) const
        {
            return os;
        }

        std::istream &operator>>(std::istream &is)
        {
            return is;
        }
    };

} // namespace org