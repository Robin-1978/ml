#pragma once

#include <vector>
#include <random>
#include <functional>
#include <cmath>
#include <memory>
#include <tuple>

namespace org
{
    struct ActivationFunction
    {
        virtual double Fun(double x) = 0;
        virtual double DFun(double y) = 0;

        using ptr = std::shared_ptr<ActivationFunction>;
    };

    struct Tanh : public ActivationFunction
    {
        double Fun(double x) override
        {
            return std::tanh(x);
        }

        double DFun(double y) override
        {
            return 1 - y * y;
        }
    };

    struct Sigmod : public ActivationFunction
    {
        double Fun(double x) override
        {
            return 1 / (1 + std::exp(-x));
        }

        double DFun(double y) override
        {
            return y * (1 - y);
        }
    };

    using values = std::vector<double>;
    using matrix = std::vector<values>;

    struct Neuron
    {
        Neuron(const unsigned count)
        {
            std::random_device rd;
            std::default_random_engine e(rd());
            std::uniform_real_distribution<double> dbDist(-1.0, 1.0);

            weights.resize(count + 1);

            for (auto &w : weights)
            {
                w = dbDist(e);
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

        values ToDna() const
        {
            return weights;
        }

        void FromDna(values::iterator &input)
        {
            for (auto &w : weights)
            {
                w = *input++;
            }
        }

        values weights;
    };

    struct Layer
    {
        std::vector<Neuron> neurons;
        ActivationFunction::ptr activate;
        Layer(const unsigned input, const unsigned output, ActivationFunction::ptr activate)
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
                results.push_back(activate->Fun(n(inputs)));
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

        values ToDna() const
        {
            values vs;
            for (auto &n : neurons)
            {
                vs.insert(vs.end(), n.weights.begin(), n.weights.end());
            }
        }
    };

    struct Network
    {
        using LayerInfo = std::tuple<unsigned, ActivationFunction::ptr>;
        std::vector<Layer> layers;

        void Construct(std::vector<LayerInfo> layerInfos)
        {
            if (layerInfos.size() < 2)
            {
                throw std::invalid_argument("layerInfos must be at least 2");
            }
            for (auto n = 0u; n < layerInfos.size() - 1; ++n)
            {
                AddLayer(std::get<0>(layerInfos[n]), std::get<0>(layerInfos[n + 1]), std::get<1>(layerInfos[n + 1]));
            }
        }

        void AddLayer(unsigned input, unsigned output, ActivationFunction::ptr activate)
        {
            layers.clear();
            layers.emplace_back(input, output, activate);
        }

        unsigned AppendLayer(unsigned output, ActivationFunction::ptr activate)
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
            result.insert(result.end(), inputs.begin(), inputs.end());
            for (auto &layer : layers)
            {
                auto layerResult = layer(*result.rbegin());
                result.insert(result.end(), layerResult.begin(), layerResult.end());
            }
            return result;
        }

        void Backword(values &labels, matrix &result, matrix &deltas)
        {
            for (auto n = 0u; n < labels.size(); n++)
            {
                auto delta = (labels[n] - result.rbegin()->at(n)) * layers.rbegin()->activate->DFun(result.rbegin()->at(n));
            }
        }

        double Loss(const values &lables, const values &outputs)
        {
            if (lables.size() != outputs.size())
            {
                std::_Throw_range_error("");
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

        values ToDna() const
        {
            values result;
            for (auto &layer : layers)
            {
                for (auto &neuron : layer.neurons)
                {
                    for (auto &weight : neuron.weights)
                    {
                        result.push_back(weight);
                    }
                }
            }
        }

        void FromDna(const values &input)
        {
        }
    };

} // namespace org