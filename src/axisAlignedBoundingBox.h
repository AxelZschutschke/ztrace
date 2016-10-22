/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_AABB_H
#define ZTRACE_AABB_H

#include "Ray.h"
#include "Traceable.h"
#include <utility>
#include <iostream>

namespace ztrace
{
Real ffmin(Real const& a, Real const& b) { return a < b ? a : b; }
Real ffmax(Real const& a, Real const& b) { return a > b ? a : b; }

class axisAlignedBoundingBox
{
public:
    axisAlignedBoundingBox()
        : min_()
        , max_()
    {
    }
    axisAlignedBoundingBox(Vector const& min, Vector const& max)
        : min_(min)
        , max_(max)
    {
    }
    axisAlignedBoundingBox(Vector&& min, Vector&& max)
        : min_(std::move(min))
        , max_(std::move(max))
    {
    }

    axisAlignedBoundingBox& operator+=(axisAlignedBoundingBox const& rhs)
    {
        for (Size i = 0; i < 3; i++) {
            min_[i] = ffmin(min_[i], rhs.min_[i]);
            max_[i] = ffmax(max_[i], rhs.max_[i]);
        }
        return *this;
    }
    axisAlignedBoundingBox operator+(axisAlignedBoundingBox const& rhs)
    {
        axisAlignedBoundingBox result = rhs;
        for (Size i = 0; i < 3; i++) {
            result.min_[i] = ffmin(min_[i], result.min_[i]);
            result.max_[i] = ffmax(max_[i], result.max_[i]);
        }
        return result;
    }

    Vector& min() { return min_; }
    Vector& max() { return max_; }
    Vector const& min() const { return min_; }
    Vector const& max() const { return max_; }

    bool hit(Ray const& rayIn, Real intervalMin, Real intervalMax) const
    {
        for (Size i = 0; i < 3; ++i) {
            Real invD = 1.0 / rayIn.direction()[i];
            Real t0 = (min_[i] - rayIn.origin()[i]) * invD;
            Real t1 = (max_[i] - rayIn.origin()[i]) * invD;
            if (invD < 0.0)
                std::swap(t0, t1);
            intervalMin = ffmax(t0, intervalMin);
            intervalMax = ffmin(t1, intervalMax);
            if (intervalMax <= intervalMin)
                return false;
        }
        return true;
    }

private:
    Vector min_;
    Vector max_;
};
}

#endif
