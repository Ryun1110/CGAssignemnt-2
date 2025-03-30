#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

class Camera {
public:
    glm::vec3 eye;
    glm::vec3 u, v, w;
    float l, r, b, t, d;

    Camera(glm::vec3 e, glm::vec3 lookAt, glm::vec3 up, float l, float r, float b, float t, float d)
        : eye(e), l(l), r(r), b(b), t(t), d(d) {
        w = glm::normalize(eye - lookAt);
        u = glm::normalize(glm::cross(up, w));
        v = glm::cross(w, u);
    }

    Ray generateRay(float i, float j, int width, int height) {
        float uCoord = l + (r - l) * (i + 0.5f) / width;
        float vCoord = b + (t - b) * (j + 0.5f) / height;
        glm::vec3 direction = glm::normalize(-d * w + uCoord * u + vCoord * v);
        return Ray(eye, direction);
    }
};

#endif
