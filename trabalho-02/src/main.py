
import glfw
import numpy as np
from OpenGL.GL import *

from engine import window as win, gl_state
from engine.mesh import Mesh
from engine import obj_loader as ld
from engine.bounds import ground_height
from engine.camera import Camera, model as model_matrix
from shaders.shaders import Shader

WIDTH, HEIGHT = 800, 600
CASTLE_SCALE = 30.0
SKY_SCALE = 100.0

# Posição inicial e limites da câmera
START_POS = (-0.35, -1.82, 10.0)
EDGE_MARGIN = 1.0
EYE_HEIGHT = 0.2
LIMIT_X_OVERRIDE = None
LIMIT_Z_OVERRIDE = None
FLOOR_Y_OVERRIDE = None
CEILING_Y_OVERRIDE = None

# Velocidades das transformações
MOVE_SPEED = 2.0
ROTATE_SPEED = 90.0
SCALE_RATE = 1.0
MOVE_LIMIT = 6.0
SCALE_MIN, SCALE_MAX = 0.3, 3.0

X, Y, Z = 0, 1, 2


def clamp(value, lo, hi):
    # Mantém o valor dentro do intervalo permitido
    return max(lo, min(hi, value))


def key_dir(window, k_neg, k_pos):
    # Retorna a direção indicada pelas teclas pressionadas
    d = 0
    if glfw.get_key(window, k_neg) == glfw.PRESS:
        d -= 1
    if glfw.get_key(window, k_pos) == glfw.PRESS:
        d += 1
    return d


class Actor:
    """Representa um modelo com transformações independentes"""

    def __init__(self, mesh, base_pos, base_scale, move=None, rotate=None, scale=None,
                 move_range=(-MOVE_LIMIT, MOVE_LIMIT)):
        self.mesh = mesh
        self.base_pos = base_pos
        self.base_scale = base_scale
        self.move, self.rotate, self.scale = move, rotate, scale
        self.move_range = move_range
        self.offset = [0.0, 0.0, 0.0]
        self.angle = 0.0
        self.scale_mult = 1.0

    def update(self, window, dt):
        # Atualiza a posição no eixo configurado
        if self.move:
            axis, k_neg, k_pos = self.move
            self.offset[axis] = clamp(
                self.offset[axis] + key_dir(window, k_neg, k_pos) * MOVE_SPEED * dt,
                *self.move_range)

        # Atualiza a rotação no eixo Y
        if self.rotate:
            self.angle = (
                self.angle + key_dir(window, *self.rotate) * ROTATE_SPEED * dt
            ) % 360.0

        # Atualiza a escala de forma uniforme
        if self.scale:
            self.scale_mult = clamp(
                self.scale_mult * (
                    1.0 + SCALE_RATE * dt * key_dir(window, *self.scale)
                ),
                SCALE_MIN, SCALE_MAX)

    def draw(self, loc_model):
        # Combina a posição inicial com o deslocamento atual
        pos = tuple(b + o for b, o in zip(self.base_pos, self.offset))
        M = model_matrix(
            angle_deg=self.angle,
            scale=self.base_scale * self.scale_mult,
            translate=pos
        )
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, M)
        self.mesh.draw()


def main():
    window = win.initWindow()
    shader = Shader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs")
    shader.use()
    program = shader.getProgram()

    gl_state.setup()
    glDisable(GL_CULL_FACE)

    # Carrega o castelo sem inverter as texturas
    castle = Mesh(program, "objects/peach_v2/castle.obj", flip=False)

    # Carrega o céu com a textura definida manualmente
    sky = Mesh(
        program,
        "objects/sky/untitled.obj",
        override_texture="objects/sky/Image_0.jpg",
        flip=True
    )

    # Configura os personagens e seus controles
    actors = [
        # Kirby pode se mover em X e alterar a escala
        Actor(
            Mesh(program, "objects/kirbyv2/untitled.obj", flip=True),
            (-0.93, -2.05, -2.29), 0.3,
            move=(X, glfw.KEY_J, glfw.KEY_L),
            scale=(glfw.KEY_MINUS, glfw.KEY_EQUAL)
        ),

        # Bandana Dee pode se mover em Z e girar
        Actor(
            Mesh(program, "objects/bandana_dee/untitled.obj", flip=True),
            (0.0, -2.1, -2.44), 0.3,
            move=(Z, glfw.KEY_I, glfw.KEY_K),
            rotate=(glfw.KEY_U, glfw.KEY_O)
        ),

        # Steve pode se mover em X
        Actor(
            Mesh(
                program,
                "objects/steve/untitled.obj",
                override_texture="objects/steve/Image_0.png",
                flip=True
            ),
            (-0.33, -1.5, -3.38), 0.3,
            move=(X, glfw.KEY_N, glfw.KEY_M)
        ),

        # Pikachu pode pular e girar
        Actor(
            Mesh(program, "objects/pikachu/untitled.obj", flip=True),
            (-0.58, -2.01, 5.44), 0.6,
            move=(Y, glfw.KEY_1, glfw.KEY_2),
            rotate=(glfw.KEY_3, glfw.KEY_4),
            move_range=(0.0, 3.0)
        ),

        # Yoshi pode se mover em Z e girar
        Actor(
            Mesh(program, "objects/yoshi/untitled.obj", flip=True),
            (-0.14, -2.08, 8.08), 0.3,
            move=(Z, glfw.KEY_5, glfw.KEY_6),
            rotate=(glfw.KEY_7, glfw.KEY_8)
        ),

        # Peach pode se mover em Z e girar
        Actor(
            Mesh(program, "objects/peach/peach.obj", flip=True),
            (-0.46, -2.0, 8.8), 0.15,
            move=(Z, glfw.KEY_9, glfw.KEY_0),
            rotate=(glfw.KEY_T, glfw.KEY_G)
        ),
    ]

    # Calcula os limites espaciais do castelo
    castle_verts = np.asarray(
        ld.load_obj("objects/peach_v2/castle.obj")[0],
        dtype=np.float32
    ).reshape(-1, 3) * CASTLE_SCALE

    lo, hi = castle_verts.min(axis=0), castle_verts.max(axis=0)

    # Define o chão pela posição dos personagens e pela malha
    floor_y = min(a.base_pos[1] for a in actors)
    under_start = ground_height(
        castle_verts,
        START_POS[0],
        START_POS[2],
        START_POS[1] + 0.05
    )

    if under_start is not None:
        floor_y = min(floor_y, under_start)

    if FLOOR_Y_OVERRIDE is not None:
        floor_y = FLOOR_Y_OVERRIDE

    # Garante que os limites incluam a posição inicial
    limit_x = LIMIT_X_OVERRIDE or (
        min(lo[0] + EDGE_MARGIN, START_POS[0]),
        max(hi[0] - EDGE_MARGIN, START_POS[0])
    )

    limit_z = LIMIT_Z_OVERRIDE or (
        min(lo[2] + EDGE_MARGIN, START_POS[2]),
        max(hi[2] - EDGE_MARGIN, START_POS[2])
    )

    limit_y = (
        min(floor_y + EYE_HEIGHT, START_POS[1]),
        CEILING_Y_OVERRIDE if CEILING_Y_OVERRIDE is not None else float(hi[1])
    )

    print(
        f"Limites da câmera: x={limit_x[0]:.2f}..{limit_x[1]:.2f}  "
        f"y={limit_y[0]:.2f}..{limit_y[1]:.2f}  "
        f"z={limit_z[0]:.2f}..{limit_z[1]:.2f}"
    )

    camera = Camera(position=START_POS)
    camera.speed = 15.0

    glfw.set_input_mode(window, glfw.CURSOR, glfw.CURSOR_DISABLED)
    glfw.set_cursor_pos_callback(window, camera.on_mouse)
    glfw.set_scroll_callback(window, camera.on_scroll)

    # Obtém as posições das variáveis uniformes dos shaders
    loc_model = glGetUniformLocation(program, "model")
    loc_view = glGetUniformLocation(program, "view")
    loc_proj = glGetUniformLocation(program, "projection")

    glfw.show_window(window)
    last = glfw.get_time()

    state = {"wireframe": False}

    def on_key(win_, key, sc, action, mods):
        if action != glfw.PRESS:
            return

        if key == glfw.KEY_Y:
            # Exibe a posição e a orientação atuais da câmera
            p = camera.pos
            print(
                f"Camera(position=({p[0]:.2f}, {p[1]:.2f}, {p[2]:.2f}), "
                f"yaw={camera.yaw:.1f}, pitch={camera.pitch:.1f})"
            )

        elif key == glfw.KEY_P:
            # Alterna entre a malha preenchida e o modo de linhas
            state["wireframe"] = not state["wireframe"]
            glPolygonMode(
                GL_FRONT_AND_BACK,
                GL_LINE if state["wireframe"] else GL_FILL
            )

    glfw.set_key_callback(window, on_key)

    print(
        "Controles: WASD/mouse = câmera | P = malha | Y = imprime câmera\n"
        "  Kirby: J/L move, -/= escala | Bandana Dee: I/K move, U/O gira\n"
        "  Steve: N/M move | Pikachu: 1/2 sobe/desce, 3/4 gira\n"
        "  Yoshi: 5/6 move, 7/8 gira | Peach: 9/0 move (Z), T/G gira"
    )

    while not glfw.window_should_close(window):
        # Calcula o tempo decorrido desde o último quadro
        now = glfw.get_time()
        dt, last = now - last, now

        glfw.poll_events()

        if glfw.get_key(window, glfw.KEY_ESCAPE) == glfw.PRESS:
            glfw.set_window_should_close(window, True)

        camera.process_keyboard(window, dt)

        # Mantém a câmera dentro dos limites da cena
        camera.pos[0] = clamp(camera.pos[0], *limit_x)
        camera.pos[1] = clamp(camera.pos[1], *limit_y)
        camera.pos[2] = clamp(camera.pos[2], *limit_z)

        for actor in actors:
            actor.update(window, dt)

        w, h = glfw.get_framebuffer_size(window)
        if h == 0:
            continue

        glViewport(0, 0, w, h)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        V = camera.view_matrix()
        P = camera.projection_matrix(w / h)

        # Remove a translação da câmera para manter o céu distante
        V_sky = V.copy()
        V_sky[:3, 3] = 0.0

      
        glDepthMask(GL_FALSE)
        glUniformMatrix4fv(
            loc_model, 1, GL_TRUE, model_matrix(scale=SKY_SCALE)
        )
        glUniformMatrix4fv(loc_view, 1, GL_TRUE, V_sky)
        glUniformMatrix4fv(loc_proj, 1, GL_TRUE, P)
        sky.draw()
        glDepthMask(GL_TRUE)

        # Desenha o castelo
        M = model_matrix(angle_deg=0.0, scale=CASTLE_SCALE)
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, M)
        glUniformMatrix4fv(loc_view, 1, GL_TRUE, V)
        glUniformMatrix4fv(loc_proj, 1, GL_TRUE, P)
        castle.draw()

        # Desenha os personagens
        for actor in actors:
            actor.draw(loc_model)

        glfw.swap_buffers(window)

    glfw.terminate()


if __name__ == "__main__":
    main()
