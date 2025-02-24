#include <stdio.h>
#include <stdlib.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "cglm/cglm.h"

#include "utils/file_read.h"

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

// need debug printf function

// initialize glfw
// intialize window
// load gl functions with glad
// load shaders
// main loop
// poll events
// handle keypresses <-- game stuff happens here
// handle resize
// clear color and depth buffer
// tell shaders data <-- game stuff happens here
// use shaders
// swap buffers
// destroy window
// terminate glfw

GLuint compile_shader(const char *file) {
  GLuint shader = glCreateShader(GL_VERTEX_SHADER);
  const char *shader_src = read_file(file);
  glShaderSource(shader, 1, &shader_src, NULL);
  glCompileShader(shader);

  int params = -1;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &params);

  if (GL_TRUE != params) {
    int max_len = 2048;
    int len = 0;
    char log[max_len];
    glGetShaderInfoLog(vs, max_len, &len, log);
    fprintf(stderr, "ERROR: shader index %u did not compile:\n%s\n", shader, log);
    exit(1);
  }
  return shader;
}

int main() {
  // initialize glfw
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

  // intialize window
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

  // load gl functions with glad
  R_glad_load_gl();

  // initialize vbos
  float points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };

  GLuint vbo = R_init_vbo();
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

  // initialize vao
  GLuint vao = R_init_vao();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // load shaders
  GLuint vs = R_compile_shader("src/shaders/main.vert");
  GLuint fs = R_compile_shader("src/shaders/main.frag");

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, fs);
  glAttachShader(shader_program, vs);
  glLinkProgram(shader_program);

  // main loop
  while (!glfwWindowShouldClose(window)) {
    // poll events
    glfwPollEvents();
    // handle keypresses
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, 1);
    }

    // handle resize
    glfwGetFramebufferSize(window, &win_w, &win_h);
    glViewport(0, 0, win_w, win_h);

    // clear color and depth buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // put the shader program and vao in the focus of the opengl state machine
    glUseProgram(shader_program);
    glBindVertexArray(vao);

    // draw
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // swap buffers
    glfwSwapBuffers(window);
  }

  // destroy window
  glfwDestroyWindow(window);
  // terminate glfw
  glfwTerminate();

  return 0;
}
