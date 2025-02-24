#include "renderer.h"

void R_glad_load_gl() {
  int glad_version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (glad_version == 0) {
    fprintf(stderr, "ERROR: Failed to initialize OpenGL context.\n");
    return 1;
  }

  printf("Renderer: %s.\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s.\n", glGetString(GL_VERSION));
}


GLuint R_init_vbo() {
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  return vbo;
}

GLuint R_init_vao(int *vao_index) {
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  return vao;
}

GLuint R_compile_shader(const char *file) {

}
