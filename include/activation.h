#pragma once
#include <cmath>
#include <memory>

namespace org
{
    namespace act
    {
        struct Activate
        {
            virtual double Calc(double x) = 0;
            virtual double Loss(double y) = 0;
            double operator()(double x)
            {
                return Calc(x);
            }
            using ptr = std::shared_ptr<Activate>;
        };

        struct Tanh : public Activate
        {
            double Calc(double x) override
            {
                return std::tanh(x);
            }

            double Loss(double y) override
            {
                return 1 - y * y;
            }
        };

        struct Sigmod : public Activate // logistic
        {
            double Calc(double x) override
            {
                return 1 / (1 + std::exp(-x));
            }

            double Loss(double y) override
            {
                return y * (1 - y);
            }
        };

        struct ReLU : public Activate
        {
            double Calc(double x) override
            {
                return x > 0.0 ? x : 0.0;
            }
            double Loss(double y) override
            {
                return 0;
            }
        };

        struct LeakyReLU : public Activate
        {
            double a;
            LeakyReLU(double a = 0.01) : a(a)
            {
            }

            double Calc(double x) override
            {
                return x > 0.0 ? x : a * x;
            }

            double Loss(double y) override
            {
                return 0;
            }
        };

        struct ELU : public Activate
        {
            double a;
            ELU(double a = 0.01) : a(a)
            {
            }

            double Calc(double x) override
            {
                return x > 0.0 ? x : a * (std::exp(x) - 1);
            }

            double Loss(double y) override
            {
                return 0;
            }
        };

        struct PReLU : public Activate
        {
            double a;
            PReLU(double a = 0.01) : a(a)
            {
            }

            double Calc(double x) override
            {
                return x > 0.0 ? x : a * x;
            }

            double Loss(double y) override
            {
                return 0;
            }
        };
        struct Softmax : public Activate
        {
            double sum;
            Softmax(double sum) : sum(sum) {}

            Softmax(std::initializer_list<double> args)
            {
                for (auto n : args)
                {
                    sum += std::exp(n);
                }
            }

            double Calc(double x) override
            {
                return std::exp(x) / sum;
            }

            double Loss(double y) override
            {
                return 0;
            }
        };

        struct Swish : public Activate
        {
            double Calc(double x) override
            {
                return x / (1 + std::exp(-x));
            }
            double Loss(double y) override
            {
                return 0;
            }
        };

        struct Softplus : public Activate
        {
            double Calc(double x) override
            {
                return std::log(1 + std::exp(x));
            }

            double Loss(double y) override
            {
                return 1 / (1 + std::exp(-y));
            }
        };
    }

}