#pragma once

#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

namespace org
{

    struct Polar;
    struct Pos
    {
        double x, y;

        double Distance2(const Pos &p) const
        {
            return ((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
        }

        double Distance(const Pos &p) const
        {
            return std::sqrt(Distance2(p));
        }

        Pos operator-(const Pos &p) const
        {
            return {x - p.x, y - p.y};
        }

        operator Polar() const;
    };

    struct Polar
    {
        double theta, rho;

        double Distance2(const Polar &p) const
        {
            return rho * rho + p.rho * p.rho - 2 * rho * p.rho * std::cos(Angle(p));
        }

        double Distance(const Polar &p) const
        {
            return std::sqrt(Distance2(p));
        }

        double Angle(const Polar &p) const
        {
            auto angle = theta - p.theta;
            if (angle > M_PI)
            {
                angle -= 2 * M_PI;
            }
            if (angle < -M_PI)
            {
                angle += 2 * M_PI;
            }
            return angle;
        }

        Polar operator-(const Polar &p) const
        {
            return {Angle(p), Distance(p)};
        }

        bool operator<(const Polar &p) const
        {
            if (rho == p.rho)
                return theta < p.theta ? true : false;

            return theta < p.theta ? true : false;
        }

        operator Pos() const
        {
            return {rho * std::cos(theta), rho * std::sin(theta)};
        }
    };


    Pos::operator Polar() const
    {
        return Polar{std::atan2(y, x), std::sqrt(x * x + y * y)};
    }
}