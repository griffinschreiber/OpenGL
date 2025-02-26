#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"

#include "utils/file_read.h"

void R_glad_load_gl() {
  int glad_version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (glad_version == 0) {
    fprintf(stderr, "ERROR: Failed to initialize OpenGL context.\n");
    exit(1);
  }

  printf("Renderer: %s.\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s.\n", glGetString(GL_VERSION));
}

GLuint R_compile_shader(const char *file, GLenum shader_type) {
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
