#ifndef SPHERE_H
#define SPHERE_H

#include "Ray.h"

class Sphere {
public:
    glm::vec3 center;
    float radius;

    Sphere(const glm::vec3& c, float r) : center(c), radius(r) {}

    bool intersect(const Ray& ray, float& t) const {
        glm::vec3 oc = ray.origin - center;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0.0f) return false;
        float temp = (-b - sqrtf(discriminant)) / (2.0f * a);
        if (temp > 0.0f) {
            t = temp;
            return true;
        }
        temp = (-b + sqrtf(discriminant)) / (2.0f * a);
        if (temp > 0.0f) {
            t = temp;
            return true;
        }
        return false;
    }

    glm::vec3 getNormal(const glm::vec3& hitPoint) const {
        return glm::normalize(hitPoint - center);
    }
};

#endif
