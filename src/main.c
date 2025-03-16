#include <stdio.h>
#include <stdlib.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "cglm/cglm.h"
#include "utils/file_read.h"


// IMPORTANT: the framebuffer is measured in pixels, but the window is measured in screen coordinates
// on some platforms these are not the same, so it is important not to confuse them.

// need debug printf function

GLFWmonitor *monitor = NULL;
int window_width = 800;
int window_height = 600;
GLFWwindow *window;
double cursor_x, cursor_y;
GLuint vao, vbo, vs, fs, shader_program;
char *vs_src, *fs_src;

void die(int exit_code) {
  glDisableVertexAttribArray(0);
  glDetachShader(shader_program, vs);
  glDetachShader(shader_program, fs);
  glDeleteProgram(shader_program);
  glDeleteShader(vs);
  glDeleteShader(fs);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  free(vs_src);
  free(fs_src);
  glfwTerminate();
  exit(exit_code);
}

void error_callback_glfw(int error, const char *msg) {
  fprintf(stderr, "GLFW ERROR: code %i, %s.\n", error, msg);
  // not sure if should exit for every error: some may be non-fatal
  die(1);
}

GLuint compile_shader(const char *shader_src, GLenum shader_type) {
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &shader_src, NULL);
  glCompileShader(shader);

  int is_compiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);

  if (is_compiled == GL_FALSE) {
    int max_len = 2048;
    char log[max_len];

    glGetShaderInfoLog(shader, max_len, NULL, log);

    fprintf(stderr, "ERROR: compile shader index %i did not compile.\n%s\n", shader, log);

    die(1);
  }

  return shader;
}

void init() {
  printf("Starting GLFW %s. \n", glfwGetVersionString());

  glfwSetErrorCallback(error_callback_glfw);

  if (!glfwInit()) {
    fprintf(stderr, "ERROR could not start GLFW.\n");
    exit(1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_SAMPLES, 4);

  // intialize window
  window = glfwCreateWindow(window_width, window_height, "Game", monitor, NULL);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "ERROR: Failed to initialize OpenGL context.\n");
    glfwTerminate();
    exit(1);
  }

  printf("Renderer: %s.\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s.\n", glGetString(GL_VERSION));

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  float points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  vs_src = read_file("src/shaders/main.vert");
  fs_src = read_file("src/shaders/main.frag");

  GLuint vs = compile_shader(vs_src, GL_VERTEX_SHADER);
  GLuint fs = compile_shader(fs_src, GL_FRAGMENT_SHADER);

  GLuint shader_program = glCreateProgram();

  glAttachShader(shader_program, vs);
  glAttachShader(shader_program, fs);

  glLinkProgram(shader_program);

  int is_linked = 0;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &is_linked);
  if (is_linked == GL_FALSE) {
    int max_len = 2048;
    char log[max_len];

    glGetProgramInfoLog(shader_program, max_len, NULL, log);

    printf("ERROR: could not link shader program.\n%s\n", log);

    die(1);
  }

  glValidateProgram(shader_program);

  int is_validated = 0;
  glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &is_validated);

  if (is_validated == GL_FALSE) {
    int max_len = 2048;
    char log[max_len];

    glGetProgramInfoLog(shader_program, max_len, NULL, log);

    printf("ERROR: validation of shader program failed.\n%s\n", log);

    die(1);
  }

  glUseProgram(shader_program);
}

int main() {
  init();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, 1);
    }

    glfwGetFramebufferSize(window, &window_width, &window_height);
    glViewport(0, 0, window_width, window_height);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
  }

  die(0);
}
