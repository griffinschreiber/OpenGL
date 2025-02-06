#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

// https://antongerdelan.net/opengl/glcontext2.html
// https://learnopengl.com/Getting-started/Coordinate-Systems
// https://github.com/recp/cglm

// min version OpenGL 3.3 is probably safe

// IMPORTANT: the framebuffer is measured in pixels, but the window is measured in screen coordinates
// on some platforms these are not the same, so it is important not to confuse them.

void error_callback_glfw(int error, const char *msg) {
  fprintf(stderr, "GLFW ERROR: code %i, %s.\n", error, msg);
  // not sure if should exit for every error: some may be non-fatal
  glfwTerminate();
  exit(1);
}

int main() {
  printf("Starting GLFW %s. \n", glfwGetVersionString());

  glfwSetErrorCallback(error_callback_glfw);

  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW.\n");
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_SAMPLES, 8);

  GLFWmonitor *monitor = NULL;

  int win_w = 800;
  int win_h = 600;

  GLFWwindow *window = glfwCreateWindow(win_w,
                                        win_h,
                                        "GLFW Template",
                                        monitor,
                                        NULL
                                        );
  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, 1);
    }

    glfwGetFramebufferSize(window, &win_w, &win_h);
    glViewport(0, 0, win_w, win_h);

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
