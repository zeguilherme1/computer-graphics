# main.py
import glfw
from OpenGL.GL import *

from engine import window as win, gl_state
from engine.mesh import Mesh
from engine.camera import Camera, model as model_matrix
from shaders.shaders import Shader

WIDTH, HEIGHT = 800, 600
CASTLE_SCALE = 30.0
SKY_SCALE = 100.0
KIRBY_SCALE = 0.3
KIRBY_POS = (-0.93, -2.05, -2.29)
BANDANA_DEE_SCALE = 0.3
BANDANA_DEE_POS = (0.0, -2.1, -2.44)
STEVE_SCALE = 0.3
STEVE_POS = (-0.33, -1.5, -3.38)
PIKACHU_SCALE = 0.6
PIKACHU_POS = (-0.58, -2.01, 5.44)
YOSHI_SCALE = 0.3
YOSHI_POS = (-0.14, -2.08, 8.08)

def main():
    window = win.initWindow()
    shader = Shader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs")
    shader.use()
    program = shader.getProgram()

    gl_state.setup()
    glDisable(GL_CULL_FACE)

    # castelo vem do glb_to_obj.py -> sem flip
    castle = Mesh(program, "objects/peach_v2/castle.obj", flip=False)
    # céu vem do Blender -> com flip; imagem passada na mão (o .mtl não tem map_Kd)
    sky = Mesh(program, "objects/sky/untitled.obj",
               override_texture="objects/sky/Image_0.jpg", flip=True)
    
    kirby = Mesh(program, "objects/kirbyv2/untitled.obj", flip=True)
    
    bandana_dee = Mesh(program, "objects/bandana_dee/untitled.obj", flip=True)
    
    steve = Mesh(program, "objects/steve/untitled.obj",
             override_texture="objects/steve/Image_0.png", flip=True)
    
    pikachu = Mesh(program, "objects/pikachu/untitled.obj", flip=True)
    
    yoshi = Mesh(program, "objects/yoshi/untitled.obj", flip=True)
    

    camera = Camera(position=(-0.35, -1.82, 10.0))
    camera.speed = 15.0
    glfw.set_input_mode(window, glfw.CURSOR, glfw.CURSOR_DISABLED)
    glfw.set_cursor_pos_callback(window, camera.on_mouse)
    glfw.set_scroll_callback(window, camera.on_scroll)

    loc_model = glGetUniformLocation(program, "model")
    loc_view = glGetUniformLocation(program, "view")
    loc_proj = glGetUniformLocation(program, "projection")

    glfw.show_window(window)
    last = glfw.get_time()

    def on_key(win_, key, sc, action, mods):
        if key == glfw.KEY_P and action == glfw.PRESS:
            p = camera.pos
            print(f"Camera(position=({p[0]:.2f}, {p[1]:.2f}, {p[2]:.2f}), "
                  f"yaw={camera.yaw:.1f}, pitch={camera.pitch:.1f})")

    glfw.set_key_callback(window, on_key)

    while not glfw.window_should_close(window):
        now = glfw.get_time()
        dt, last = now - last, now

        glfw.poll_events()
        if glfw.get_key(window, glfw.KEY_ESCAPE) == glfw.PRESS:
            glfw.set_window_should_close(window, True)
        camera.process_keyboard(window, dt)

        w, h = glfw.get_framebuffer_size(window)
        if h == 0:
            continue
        glViewport(0, 0, w, h)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        V = camera.view_matrix()
        P = camera.projection_matrix(w / h)

        # --- céu ---
        V_sky = V.copy()
        V_sky[:3, 3] = 0.0
        glDepthMask(GL_FALSE)
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, model_matrix(scale=SKY_SCALE))
        glUniformMatrix4fv(loc_view, 1, GL_TRUE, V_sky)
        glUniformMatrix4fv(loc_proj, 1, GL_TRUE, P)
        sky.draw()
        glDepthMask(GL_TRUE)

        # --- castelo ---
        M = model_matrix(angle_deg=0.0, scale=CASTLE_SCALE)
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, M)
        glUniformMatrix4fv(loc_view, 1, GL_TRUE, V)
        glUniformMatrix4fv(loc_proj, 1, GL_TRUE, P)
        castle.draw()

        # --- kirby ---
        M_kirby = model_matrix(angle_deg=glfw.get_time() * 0.0,  # gira; use 0.0 para ficar parado
                               scale=KIRBY_SCALE,
                               translate=KIRBY_POS)
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, M_kirby)
        kirby.draw()
        
        M_bandana_dee = model_matrix(angle_deg=glfw.get_time() * 0.0,  # gira; use 0.0 para ficar parado
                                       scale=BANDANA_DEE_SCALE,
                                       translate=BANDANA_DEE_POS)
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, M_bandana_dee)
        bandana_dee.draw()
        
        M_steve = model_matrix(angle_deg=glfw.get_time() * 0.0,  # gira; use 0.0 para ficar parado
                                               scale=STEVE_SCALE,
                                               translate=STEVE_POS)
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, M_steve)
        steve.draw()
        
        M_pikachu = model_matrix(angle_deg=glfw.get_time() * 0.0,  # gira; use 0.0 para ficar parado
                                                    scale=PIKACHU_SCALE,
                                                    translate=PIKACHU_POS)
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, M_pikachu)
        pikachu.draw()
        
        M_yoshi = model_matrix(angle_deg=glfw.get_time() * 0.0,  # gira; use 0.0 para ficar parado
                                                    scale=YOSHI_SCALE,
                                                    translate=YOSHI_POS)
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, M_yoshi)
        yoshi.draw()
        
        
        glfw.swap_buffers(window)

    glfw.terminate()


if __name__ == "__main__":
    main()