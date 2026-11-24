import numpy as np
from OpenGL.GL import *

from engine.camera import model as model_matrix

# ambiente ao qual o objeto pertence
INDOOR, OUTDOOR, BOTH = 0, 1, 2


class Material:
    """Parâmetros de iluminação próprios de cada objeto"""

    def __init__(self, ka=1.0, kd=0.8, ks=0.3, shininess=32.0):
        self.ka, self.kd, self.ks, self.shininess = ka, kd, ks, shininess


class Actor:
    """Modelo posicionado na cena, com material, ambiente e animação opcional"""

    def __init__(self, mesh, base_pos, base_scale, material=None,
                 env=INDOOR, spin=0.0, animate=None):
        self.mesh = mesh
        self.base_pos = base_pos
        self.base_scale = base_scale
        self.material = material or Material()
        self.env = env
        self.spin = spin          
        self.animate = animate   
        self.offset = (0.0, 0.0, 0.0)
        self.angle = 0.0
        self.emissive = (0.0, 0.0, 0.0)  # brilho próprio

    def update(self, t, dt):
        self.angle = (self.angle + self.spin * dt) % 360.0
        if self.animate:
            self.offset = self.animate(t)

    def world_pos(self):
        return tuple(b + o for b, o in zip(self.base_pos, self.offset))

    def model(self):
        return model_matrix(
            angle_deg=self.angle,
            scale=self.base_scale,
            translate=self.world_pos()
        )

    def center_world(self):
        """Centro geométrico da malha no mundo (com escala, rotação e translação)"""
        c = np.append(self.mesh.center, 1.0).astype(np.float32)
        return tuple((self.model() @ c)[:3])

    def draw(self, shader):
        shader.setMat4("model", self.model())
        apply_material(shader, self.material, self.env, self.emissive)
        self.mesh.draw()


def apply_material(shader, mat, env, emissive=(0.0, 0.0, 0.0)):
    """Envia material, ambiente e emissão do objeto ao shader"""
    shader.setFloat("matKa", mat.ka)
    shader.setFloat("matKd", mat.kd)
    shader.setFloat("matKs", mat.ks)
    shader.setFloat("matShininess", mat.shininess)
    shader.setInt("objEnv", env)
    shader.setVec3("emissive", emissive)