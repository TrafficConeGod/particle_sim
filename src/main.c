#include "gfx.h"
#include "sim.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <time.h>

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

    if (gfx_init() != 0) {
        goto error;
    }
    sim_init(win);

    size_t tick_count = 0;

    for (;;) {
        #ifdef TIMINGS
        clock_t begin_sim = clock();
        sim_update(1.0f/1280.0f, 1.0f/720.0f, win, tick_count);
        clock_t end_sim_begin_gfx = clock();
        gfx_update();
        clock_t end_gfx = clock();
        
        clock_t sim_time = end_sim_begin_gfx - begin_sim;
        clock_t gfx_time = end_gfx - end_sim_begin_gfx;
        clock_t total_time = end_gfx - begin_sim;
        printf("%ld, %ld, %ld\n", sim_time, gfx_time, total_time);
        #else
        sim_update(1.0f/1280.0f, 1.0f/720.0f, win, tick_count);
        gfx_update();
        #endif

        tick_count++;

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