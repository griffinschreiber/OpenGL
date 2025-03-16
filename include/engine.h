#ifndef ENGINE_H_
#define ENGINE_H_

enum engine_shader_type {
  ENGINE_VERT_SHADER,
  ENGINE_FRAG_SHADER
};

typedef struct {

} engine_state;

int engine_init(engine_state *state);

void engine_window_init(engine_state *state, engine_window *window);

int engine_add_shader(engine_state *state, int shader_type);

int engine_poll_events(engine_state *state);

int engine_handle_resize(engine_state *state);

int engine_draw(engine_state *state);

#endif // ENGINE_H_
