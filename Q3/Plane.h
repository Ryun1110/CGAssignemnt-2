#ifndef PLANE_H
#define PLANE_H

#include "Ray.h"

class Plane {
public:
    float y;

    Plane(float yPos) : y(yPos) {}


    bool intersect(const Ray& ray, float& t) const {

        if (ray.direction.y == 0.0f) return false;
        t = (y - ray.origin.y) / ray.direction.y;
        return (t > 0.0f);
    }



    glm::vec3 getNormal() const {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }
};

#endif
