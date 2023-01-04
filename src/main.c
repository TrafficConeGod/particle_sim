#include "gfx.h"
#include "sim.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

int main() {
    // Init GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        goto error;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(1280, 720, "Test", NULL, NULL);
    if (win == NULL) {
        glfwTerminate();
        printf("Failed to open GLFW window\n");
        goto error;
    }   
    glfwMakeContextCurrent(win);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        goto error;
    }

    glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);

    if (gfx_init() != 0) {
        goto error;
    }
    sim_init();

    for (;;) {
        sim_update();
        gfx_update();

        glfwSwapBuffers(win);

        glfwPollEvents();

        if (glfwWindowShouldClose(win)) {
            goto end;
        }
    }

    error:
        return 1;
    end:
        return 0;
}