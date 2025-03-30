#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <random>
#include "Ray.h"
#include "Camera.h"
#include "Plane.h"
#include "Scene.h"
#include "Sphere.h"
#include "Material.h"

using namespace glm;

// -------------------------------------------------
// Global Variables
// -------------------------------------------------
int Width = 512;
int Height = 512;
std::vector<float> OutputImage;
// -------------------------------------------------

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

void render(Camera& camera, Scene& scene) {
    OutputImage.clear();
    OutputImage.reserve(Width * Height * 3);

    int N = 64;
    float invN = 1.0f / float(N);

    float gamma = 2.2f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int j = 0; j < Height; ++j) {
        for (int i = 0; i < Width; ++i) {
            glm::vec3 colorSum(0.0f);

            for (int sample = 0; sample < N; sample++) {
                float rx = dis(gen);
                float ry = dis(gen);

                float px = i + rx;
                float py = j + ry;

                Ray ray = camera.generateRay(px, py, Width, Height);

                float t;
                glm::vec3 colorOneSample(0.0f);
                if (scene.intersect(ray, t, colorOneSample)) {
                    colorSum += colorOneSample;
                }
                else {
                }
            }

            glm::vec3 color = colorSum * invN;

            color.r = glm::clamp(color.r, 0.0f, 1.0f);
            color.g = glm::clamp(color.g, 0.0f, 1.0f);
            color.b = glm::clamp(color.b, 0.0f, 1.0f);

            color = glm::pow(color, glm::vec3(1.0f / gamma));

            OutputImage.push_back(color.r);
            OutputImage.push_back(color.g);
            OutputImage.push_back(color.b);
        }
    }
}



Camera camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), -0.1, 0.1, -0.1, 0.1, 0.1);
Scene scene;

void resize_callback(GLFWwindow*, int nw, int nh) {
    Width = nw;
    Height = nh;
    glViewport(0, 0, nw, nh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, static_cast<double>(Width), 0.0, static_cast<double>(Height), 1.0, -1.0);
    OutputImage.reserve(Width * Height * 3);
    render(camera, scene);
}

int main(int argc, char* argv[]) {
    GLFWwindow* window;

    if (!glfwInit()) return -1;

    window = glfwCreateWindow(Width, Height, "Ray Tracer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    Camera camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), -0.1, 0.1, -0.1, 0.1, 0.1);
    Scene scene;
    render(camera, scene);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawPixels(Width, Height, GL_RGB, GL_FLOAT, &OutputImage[0]);
        glfwSwapBuffers(window);
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;


}
