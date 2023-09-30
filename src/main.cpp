#include <iostream>

// use (modern) OpenGL
#define GLEW_STATIC
#include <GL/glew.h>

// windowing and input
#include <GLFW/glfw3.h>

// utilities for creating windows etc.
#include "util.hpp"
#include "shader_s.hpp"

struct Vertex {
    float x;
    float y;
    float z;
    float u;
    float v;
};

const Vertex vertices[] = {
    Vertex {
        .x = -1.0,
        .y = -1.0,
        .z = 0.0,
        .u = -1.0,
        .v = -1.0,
    },
    Vertex {
        .x =  1.0,
        .y = -1.0,
        .z = 0.0,
        .u =  1.0,
        .v = -1.0,
    },
    Vertex {
        .x =  1.0,
        .y =  1.0,
        .z =  0.0,
        .u =  1.0,
        .v =  1.0,
    },
    Vertex {
        .x = -1.0,
        .y =  1.0,
        .z =  0.0,
        .u = -1.0,
        .v =  1.0,
    },
    Vertex {
        .x =  1.0,
        .y =  1.0,
        .z =  0.0,
        .u =  1.0,
        .v =  1.0,
    },
    Vertex {
        .x = -1.0,
        .y = -1.0,
        .z =  0.0,
        .u = -1.0,
        .v = -1.0,
    },
};

struct Vec3 {
    float x;
    float y;
    float z;
};

int main(int argc, char** argv)
{
    Vec3 pos = {0.0f};
    GLFWwindow* window = setup();

    unsigned int VAO;
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glCreateBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    Shader shader("./vertex.glsl", "./fragment.glsl");
    shader.use();

    unsigned int cam_pos = glGetUniformLocation(shader.ID, "cam_pos");

    float currentFrame = glfwGetTime();
    float lastFrame = 0.0f;

    float fastest = 1000.0;
    float slowest = 0.0;
    float avg = 0.0;

    unsigned long frame_count = 0;

    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();

        glUniform3fv(cam_pos, 1, (float*)(&pos));

        if (frame_count > 5) {
            fastest = std::min(fastest, deltaTime);
            slowest = std::max(slowest, deltaTime);
            avg = (avg*frame_count + deltaTime) / (frame_count + 1);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            pos.z += 10.0 * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            pos.z -= 10.0 * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            pos.x += 10.0 * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            pos.x -= 10.0 * deltaTime;

        frame_count++;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    std::cout << "MAX FPS: " << 1.0 / fastest << std::endl;
    std::cout << "AVG FPS: " << 1.0 / avg << std::endl;
    std::cout << "MIN FPS: " << 1.0 / slowest << std::endl;

    terminate();
}
