#ifndef RENDERER_H_
#define RENDERER_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "utils/file_read.h"

void R_glad_load_gl();

GLuint R_init_vbo();

GLuint R_init_vao();

GLuint R_compile_shader(const char *file);

#endif // RENDERER_H_
