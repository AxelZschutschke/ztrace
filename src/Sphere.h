//
// Created by Axel Zschutschke on 10/9/16.
//

#ifndef ZTRACER_SPHERE_H
#define ZTRACER_SPHERE_H

#include "Traceable.h"

namespace ztrace {
    class Sphere : public Traceable {
        using MaterialPtr = std::shared_ptr<Material>;
    public:
        Sphere() = default;

        Sphere(Vector const &center, Real const &radius, MaterialPtr material = nullptr )
                : center_(center), radius_(radius), material_(material) {}

        static bool checkHitInterval(Real const &positionOnRay, Real const &intervalLower, Real const &intervalUpper ) {
            if (positionOnRay < intervalLower or positionOnRay > intervalUpper) {
                return false;
            }
            return true;
        }

        bool hit(Ray const &ray, Real const &intervalLower, Real const &intervalUpper, TraceData &traceData) const {
            Vector oc = ray.origin() - center_;
            Real a = dot(ray.direction(), ray.direction());
            Real b = 2.0 * dot(oc, ray.direction());
            Real c = dot(oc, oc) - radius_ * radius_;
            Real discriminant = (b * b - 4. * a * c);
            if (discriminant <= 0.) {
                return false;
            }

            Real positionOnRay = (-b - sqrt(discriminant)) / (2.0 * a);
            if (!checkHitInterval(positionOnRay, intervalLower, intervalUpper)) {
                return false;
            }

            traceData.positionOnRay = positionOnRay;
            traceData.point = ray.positionLength(traceData.positionOnRay);
            traceData.normal = traceData.point - center_;
            traceData.normal.makeUnitVector();
            traceData.material = material_;
            return true;
        }

        Vector const &center() const { return center_; }

        Real const &radius() const { return radius_; }

    private:
        Vector center_;
        Real radius_;
        MaterialPtr material_;
    };
}

#endif //ZTRACER_SPHERE_H
