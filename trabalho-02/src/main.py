# main.py
import glfw
from OpenGL.GL import *

from engine import window as win, gl_state
from engine.mesh import Mesh
from engine.input import process_input
from engine.transform import model_matrix, view_matrix, projection_matrix, to_np
from shaders.shaders import Shader

WIDTH, HEIGHT = 800, 600  

def main():
    window = win.initWindow()
    shader = Shader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs")
    shader.use()
    program = shader.getProgram()

    gl_state.setup()
    patrick = Mesh(program, "objects/patrick/patrick.obj",
                   ["objects/patrick/patrick.png"])

    state = {"angle": 0.0, "speed": 0.03}

    loc_model = glGetUniformLocation(program, "model")
    loc_view  = glGetUniformLocation(program, "view")
    loc_proj  = glGetUniformLocation(program, "projection")

    glUniformMatrix4fv(loc_view, 1, GL_FALSE, to_np(view_matrix(5.0)))
    glUniformMatrix4fv(loc_proj, 1, GL_FALSE, to_np(projection_matrix(WIDTH, HEIGHT)))

    glfw.show_window(window)
    while not glfw.window_should_close(window):
        glfw.poll_events()
        process_input(window, state)

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        model = model_matrix(state["angle"], scale=0.025)
        glUniformMatrix4fv(loc_model, 1, GL_FALSE, to_np(model))

        patrick.draw()
        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()