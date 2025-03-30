#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

struct Material {
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float     shininess;

    Material()
        : ka(0.0f), kd(0.0f), ks(0.0f), shininess(0.0f) {
    }

    Material(const glm::vec3& a, const glm::vec3& d,
        const glm::vec3& s, float sh)
        : ka(a), kd(d), ks(s), shininess(sh) {
    }
};

#endif
