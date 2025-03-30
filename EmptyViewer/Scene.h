#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <limits>
#include <cmath>

#include "Plane.h"
#include "Sphere.h"
#include "Ray.h"
#include "Material.h"

struct HitInfo {
    bool hit;
    float t;
    glm::vec3 normal;
    glm::vec3 hitPoint;
    int objectID;
    bool isPlane;
};

class Scene {
public:
    Plane plane;
    Material planeMat;

    std::vector<Sphere> spheres;
    std::vector<Material> sphereMats;

    glm::vec3 lightPos = glm::vec3(-4.0f, 4.0f, -3.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    float lightIntensity = 1.0f;

    Scene() : plane(-2.0f) {
        spheres.emplace_back(glm::vec3(-4, 0, -7), 1);
        spheres.emplace_back(glm::vec3(0, 0, -7), 2);
        spheres.emplace_back(glm::vec3(4, 0, -7), 1);

        planeMat = Material(
            glm::vec3(0.2f, 0.2f, 0.2f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            0.0f
        );

        sphereMats.resize(3);
        sphereMats[0] = Material(
            glm::vec3(0.2f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            0.0f
        );
        sphereMats[1] = Material(
            glm::vec3(0.0f, 0.2f, 0.0f),
            glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            32.0f
        );
        sphereMats[2] = Material(
            glm::vec3(0.0f, 0.0f, 0.2f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            0.0f
        );
    }

    bool findClosestIntersection(const Ray& ray, HitInfo& outHit) const {
        outHit.hit = false;
        outHit.t = FLT_MAX;

        for (int i = 0; i < (int)spheres.size(); i++) {
            float tSphere;
            if (spheres[i].intersect(ray, tSphere)) {
                if (tSphere > 0.0f && tSphere < outHit.t) {
                    outHit.hit = true;
                    outHit.t = tSphere;
                    outHit.objectID = i;
                    outHit.isPlane = false;
                }
            }
        }

        float tPlane;
        if (plane.intersect(ray, tPlane)) {
            if (tPlane > 0.0f && tPlane < outHit.t) {
                outHit.hit = true;
                outHit.t = tPlane;
                outHit.objectID = -1;
                outHit.isPlane = true;
            }
        }

        if (outHit.hit) {
            outHit.hitPoint = ray.origin + outHit.t * ray.direction;
            if (outHit.isPlane) {
                outHit.normal = plane.getNormal();
            }
            else {
                outHit.normal = spheres[outHit.objectID].getNormal(outHit.hitPoint);
            }
        }
        return outHit.hit;
    }

    bool inShadow(const glm::vec3& point, const glm::vec3& lightDir) const {
        float epsilon = 1e-4f;
        glm::vec3 shadowOrigin = point + epsilon * lightDir;

        Ray shadowRay(shadowOrigin, lightDir);
        HitInfo tempHit;
        if (!findClosestIntersection(shadowRay, tempHit))
            return false;

        float distToLight = glm::length(lightPos - point);
        if (tempHit.t > 0.0f && tempHit.t < distToLight) {
            return true;
        }
        return false;
    }

    glm::vec3 computePhong(const Ray& ray, const HitInfo& hit) const {
        Material mat;
        if (hit.isPlane) {
            mat = planeMat;
        }
        else {
            mat = sphereMats[hit.objectID];
        }

        glm::vec3 color = mat.ka;
        glm::vec3 lightDir = glm::normalize(lightPos - hit.hitPoint);

        bool shadowed = inShadow(hit.hitPoint, lightDir);
        if (shadowed) {
            return color;
        }

        float diff = glm::max(glm::dot(hit.normal, lightDir), 0.0f);
        color += mat.kd * diff;

        glm::vec3 viewDir = glm::normalize(ray.origin - hit.hitPoint);
        glm::vec3 reflectDir = glm::reflect(-lightDir, hit.normal);
        float specFactor = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), mat.shininess);
        color += mat.ks * specFactor;

        color *= lightColor * lightIntensity;

        return color;
    }

    bool intersect(const Ray& ray, float& t, glm::vec3& outColor) const {
        HitInfo hitInfo;
        if (!findClosestIntersection(ray, hitInfo)) {
            return false;
        }
        outColor = computePhong(ray, hitInfo);
        t = hitInfo.t;
        return true;
    }
};

#endif
