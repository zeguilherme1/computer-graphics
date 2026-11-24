import glfw

def process_input(window, state, dt=1.0):
    if glfw.get_key(window, glfw.KEY_ESCAPE) == glfw.PRESS:
        glfw.set_window_should_close(window, True)
    if glfw.get_key(window, glfw.KEY_RIGHT) == glfw.PRESS:
        state["angle"] += state["speed"]
    if glfw.get_key(window, glfw.KEY_LEFT) == glfw.PRESS:
        state["angle"] -= state["speed"]