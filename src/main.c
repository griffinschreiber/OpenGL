#include <stdio.h>
#include <stdlib.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "cglm/cglm.h"

#include "utils/file_read.h"


// IMPORTANT: the framebuffer is measured in pixels, but the window is measured in screen coordinates
// on some platforms these are not the same, so it is important not to confuse them.

void error_callback_glfw(int error, const char *msg) {
  fprintf(stderr, "GLFW ERROR: code %i, %s.\n", error, msg);
  // not sure if should exit for every error: some may be non-fatal
  glfwTerminate();
  exit(1);
}

GLuint compile_shader(const char *file, GLenum shader_type) {
  const char *contents = read_file(file);
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &contents, NULL);
  glCompileShader(shader);

  int params = -1;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &params);

  if (params != GL_TRUE) {
    int max = 2048;
    int len = 0;
    char log[max];

    glGetShaderInfoLog(shader, max, &len, log);
    fprintf(stderr, "ERROR: shader index %u (\"%s\") did not compile.\n%s\n", shader, file, log);
    exit(1);
  }

  return shader;
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

  // intialize window
  GLFWmonitor *monitor = NULL;

  int width = 800;
  int height = 600;

  GLFWwindow *window = glfwCreateWindow(width,
                                        height,
                                        "Game",
                                        monitor,
                                        NULL
                                        );
  glfwMakeContextCurrent(window);

   int glad_version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (glad_version == 0) {
    fprintf(stderr, "ERROR: Failed to initialize OpenGL context.\n");
    exit(1);
  }

  printf("Renderer: %s.\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s.\n", glGetString(GL_VERSION));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  float points[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
  };

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  GLuint vs = compile_shader("src/shaders/main.vert", GL_VERTEX_SHADER);
  GLuint fs = compile_shader("src/shaders/main.frag", GL_FRAGMENT_SHADER);

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, fs);
  glAttachShader(shader_program, vs);
  glLinkProgram(shader_program);

  double cursor_x, cursor_y;

  vec3 pos = {-3, 0, 0};
  vec3 dir = {1, 0, 0};
  vec3 up = {0, 1, 0};

  /* float pitch = 0; */
  /* float yaw = 0; */

  mat4 projection, view, model, mvp;
  glm_perspective(glm_rad(75), (float)width / height, 0.1, 100, projection);
  glm_lookat(glm_vec3_add(pos, dir), pos, up, view);
  glm_mat4_identity(model);
  glm_mat4_mulN({projection, view, model}, 3, mvp);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, 1);
    }

    glfwGetCursorPos(window, &cursor_x, &cursor_y);

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int mvp_loc = glGetUniformLocation(shader_program, "mvp");

    glUseProgram(shader_program);
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
