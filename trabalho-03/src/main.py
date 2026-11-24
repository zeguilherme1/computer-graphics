import math
import glfw
import numpy as np
from OpenGL.GL import *

from engine import window as win
from engine.mesh import Mesh
from engine import obj_loader as ld
from engine.camera import Camera, model as model_matrix
from engine.actor import Actor, Material, INDOOR, OUTDOOR, BOTH, apply_material
from engine.light import LightSource
from engine.utils import clamp, gl_state_setup, key_dir
from shaders.shaders import Shader


WIDTH, HEIGHT = 800, 600
CASTLE_SCALE = 4.0
SKY_SCALE = 100.0
GROUND_SCALE = 50.0

FLOOR_Y = -5.8
GROUND_EPS = 0.01

CASTLE_PATH = "objects/castle/castelinho.obj"

START_POS = (0.0, -5.8, 5.0)
EYE_HEIGHT = 0.2
LIMIT_X_OVERRIDE = (-2.2, 2.2)
LIMIT_Z_OVERRIDE = (-7.0, 7.0)
LIMIT_Y_OVERRIDE = (-5.2, -2.6)
EDGE_MARGIN = 1.0

WHITE = (1.0, 1.0, 1.0)
YELLOW = (1.0, 0.85, 0.2)
STREET = (0.8, 0.9, 1.0)

LEVEL_RATE = 0.5
AMBIENT_MAX, LEVEL_MAX = 1.0, 3.0


def main():
    window = win.initWindow()
    shader = Shader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs")
    shader.use()
    program = shader.getProgram()

    gl_state_setup()

    castle = Mesh(program, CASTLE_PATH, flip=False)

    sky = Mesh(
        program,
        "objects/night_sky/sky.obj",
        override_texture="objects/night_sky/Image.png",
        flip=True
    )

    ground = Mesh(
        program,
        "objects/ground/ground.obj",
        override_texture="objects/ground/Image_1.003.jpg",
        flip=True
    )

    mat_castle = Material(ka=1.0, kd=0.8, ks=0.15, shininess=16)
    mat_ground = Material(ka=1.0, kd=0.7, ks=0.05, shininess=8)

    actors = [
        Actor(
            Mesh(program, "objects/kirbyv2/untitled.obj", flip=True),
            (-0.5, -5.7, -0.3), 0.3,
            Material(1.0, 0.9, 0.5, 64), INDOOR
        ),

        Actor(
            Mesh(program, "objects/bandana_dee/untitled.obj", flip=True),
            (0.7, -5.7, -0.3), 0.3,
            Material(1.0, 0.8, 0.1, 4), INDOOR
        ),

        Actor(
            Mesh(
                program,
                "objects/steve/untitled.obj",
                override_texture="objects/steve/Image_0.png",
                flip=True
            ),
            (-0.0, -5.6, -0.5), 0.3,
            Material(1.0, 0.8, 0.1, 8), INDOOR
        ),

        Actor(
            Mesh(program, "objects/pikachu/pikachu.obj", flip=True),
            (0.4, -5.6, 2.3), 0.6,
            Material(1.0, 0.9, 0.4, 48), OUTDOOR
        ),

        Actor(
            Mesh(program, "objects/yoshi/untitled.obj", flip=True),
            (-0.4, -5.6, 2.3), 0.3,
            Material(1.0, 0.85, 0.4, 48), OUTDOOR
        ),

        Actor(
            Mesh(program, "objects/peach/peach.obj", flip=True),
            (-0.8, -5.6, 3.0), 0.4,
            Material(1.0, 0.8, 0.6, 96), OUTDOOR
        ),

        Actor(
            Mesh(program, "objects/star/star.obj", flip=True),
            (-0.3, -5.1, -0.3), 0.1,
            Material(1.0, 0.8, 0.8, 128), INDOOR, spin=60.0
        ),

        Actor(
            Mesh(program, "objects/chandelier/untitled.obj", flip=True),
            (-0.0, -5.1, -0.2), 0.3,
            Material(1.0, 0.8, 0.8, 128), INDOOR
        ),

        Actor(
            Mesh(program, "objects/light_pole/untitled.obj", flip=True),
            (-0.0, -5.6, 3.0), 0.4,
            Material(1.0, 0.7, 0.1, 8), OUTDOOR,
            animate=lambda t: (1.8 * math.sin(0.6 * t), 0.0, 0.0)
        ),
        Actor(
            Mesh(program, "objects/tree/tree.obj", flip=True),
            (-2.2, -5.4, 3.0), 0.7,
            Material(1.0, 0.7, 0.1, 8), OUTDOOR,
        ),
        Actor(
            Mesh(program, "objects/tree/tree.obj", flip=True),
            (2.2, -5.4, 3.0), 0.7,
            Material(1.0, 0.7, 0.1, 8), OUTDOOR,
        ),
    ]

    star = actors[6]
    chandelier = actors[7]
    pole = actors[8]

    lights = [
        LightSource(chandelier, WHITE, INDOOR, offset=(0.0, 0.2, 0.0)),
        LightSource(star, YELLOW, INDOOR, offset=(0.0, 0.0, 0.0), use_center=True),
        LightSource(pole, STREET, OUTDOOR, offset=(0.0, 0.8, 0.0)),
    ]

    castle_local = np.asarray(
        ld.load_obj(CASTLE_PATH)[0], dtype=np.float32
    ).reshape(-1, 3) * CASTLE_SCALE

    castle_y = FLOOR_Y - float(castle_local[:, 1].min())
    castle_offset = (0.0, castle_y, 0.0)

    castle_world = castle_local + np.array(castle_offset, dtype=np.float32)
    lo, hi = castle_world.min(axis=0), castle_world.max(axis=0)

    M_ground = model_matrix(
        scale=GROUND_SCALE,
        translate=(
            float((lo[0] + hi[0]) / 2),
            FLOOR_Y - GROUND_EPS,
            float((lo[2] + hi[2]) / 2)
        )
    )
    M_castle = model_matrix(
        angle_deg=0.0, scale=CASTLE_SCALE, translate=castle_offset
    )

    limit_x = LIMIT_X_OVERRIDE or (
        min(lo[0] + EDGE_MARGIN, START_POS[0]),
        max(hi[0] - EDGE_MARGIN, START_POS[0])
    )
    limit_z = LIMIT_Z_OVERRIDE or (
        min(lo[2] + EDGE_MARGIN, START_POS[2]),
        max(hi[2] - EDGE_MARGIN, START_POS[2])
    )
    limit_y = LIMIT_Y_OVERRIDE or (
        min(FLOOR_Y + EYE_HEIGHT, START_POS[1]),
        float(hi[1])
    )

    camera = Camera(position=START_POS)
    camera.speed = 10.0

    glfw.set_input_mode(window, glfw.CURSOR, glfw.CURSOR_DISABLED)
    glfw.set_cursor_pos_callback(window, camera.on_mouse)
    glfw.set_scroll_callback(window, camera.on_scroll)

    state = {"ambient_on": True, "ambient": 0.25, "diffuse": 1.0, "specular": 1.0}

    def on_key(win_, key, sc, action, mods):
        if action != glfw.PRESS:
            return
        if key == glfw.KEY_1:
            lights[0].toggle()
        elif key == glfw.KEY_2:
            lights[1].toggle()
        elif key == glfw.KEY_3:
            lights[2].toggle()
        elif key == glfw.KEY_4:
            state["ambient_on"] = not state["ambient_on"]

    glfw.set_key_callback(window, on_key)

    print(
        "Controles: WASD/mouse = câmera\n"
        "1 = lustre | 2 = estrela | 3 = poste | 4 = luz ambiente (liga/desliga)\n"
        "R/F = ambiente +/- | T/G = difusa +/- | Y/H = especular +/-"
    )

    glfw.show_window(window)
    last = glfw.get_time()

    while not glfw.window_should_close(window):
        now = glfw.get_time()
        dt, last = now - last, now

        glfw.poll_events()

        if glfw.get_key(window, glfw.KEY_ESCAPE) == glfw.PRESS:
            glfw.set_window_should_close(window, True)

        camera.process_keyboard(window, dt)

        camera.pos[0] = clamp(camera.pos[0], *limit_x)
        camera.pos[1] = clamp(camera.pos[1], *limit_y)
        camera.pos[2] = clamp(camera.pos[2], *limit_z)

        state["ambient"] = clamp(
            state["ambient"] + key_dir(window, glfw.KEY_F, glfw.KEY_R) * LEVEL_RATE * dt,
            0.0, AMBIENT_MAX
        )
        state["diffuse"] = clamp(
            state["diffuse"] + key_dir(window, glfw.KEY_G, glfw.KEY_T) * LEVEL_RATE * dt,
            0.0, LEVEL_MAX
        )
        state["specular"] = clamp(
            state["specular"] + key_dir(window, glfw.KEY_H, glfw.KEY_Y) * LEVEL_RATE * dt,
            0.0, LEVEL_MAX
        )

        for actor in actors:
            actor.update(now, dt)
        for light in lights:
            light.update()

        w, h = glfw.get_framebuffer_size(window)
        if h == 0:
            continue

        glViewport(0, 0, w, h)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        V = camera.view_matrix()
        P = camera.projection_matrix(w / h)

        V_sky = V.copy()
        V_sky[:3, 3] = 0.0

        shader.setVec3("viewPos", camera.pos)
        shader.setFloat(
            "ambientLevel",
            state["ambient"] if state["ambient_on"] else 0.0
        )
        shader.setFloat("diffuseLevel", state["diffuse"])
        shader.setFloat("specularLevel", state["specular"])
        for i, light in enumerate(lights):
            light.upload(shader, i)

        glDepthMask(GL_FALSE)
        shader.setInt("unlit", 1)
        shader.setMat4("model", model_matrix(scale=SKY_SCALE))
        shader.setMat4("view", V_sky)
        shader.setMat4("projection", P)
        sky.draw()
        shader.setInt("unlit", 0)
        glDepthMask(GL_TRUE)

        shader.setMat4("view", V)
        shader.setMat4("projection", P)

        shader.setMat4("model", M_ground)
        apply_material(shader, mat_ground, OUTDOOR)
        ground.draw()

        shader.setMat4("model", M_castle)
        apply_material(shader, mat_castle, BOTH)
        castle.draw()

        for actor in actors:
            actor.draw(shader)

        glfw.swap_buffers(window)

    glfw.terminate()


if __name__ == "__main__":
    main()