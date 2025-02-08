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


void compile_shader(GLuint shader) {

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
  int glad_version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (glad_version == 0) {
    fprintf(stderr, "ERROR: Failed to initialize OpenGL context.\n");
    return 1;
  }

  printf("Renderer: %s.\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s.\n", glGetString(GL_VERSION));

  // initialize vbos
  float points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

  // initialize vao
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // load shaders
  const char *vert_shader = read_file("src/shaders/main.vert");
  const char *frag_shader = read_file("src/shaders/main.frag");

  /* printf("DEBUG: vertex shader: \n%s\n", vert_shader); */
  /* printf("DEBUG: fragment shader: \n%s\n", frag_shader); */

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vert_shader, NULL);
  glCompileShader(vs);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &frag_shader, NULL);
  glCompileShader(fs);

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
