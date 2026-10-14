from OpenGL.GL import glUniformMatrix4fv, GL_TRUE

from engine.camera import model as model_matrix
from engine.utils import clamp, key_dir

# Velocidades das transformações
MOVE_SPEED = 2.0
ROTATE_SPEED = 90.0
SCALE_RATE = 1.0
MOVE_LIMIT = 6.0
SCALE_MIN, SCALE_MAX = 0.3, 3.0

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