import numpy as np


class LightSource:
    """Fonte de luz pontual acoplada a um Actor (lustre, estrela, poste)"""

    def __init__(self, actor, color, env, offset=(0.0, 0.0, 0.0),
                 use_center=False):
        self.actor = actor
        self.color = np.array(color, dtype=np.float32)
        self.env = env                # só afeta objetos deste ambiente
        self.offset = offset         
        self.use_center = use_center  # true -> parte do centro da malha
        self.on = True

    def toggle(self):
        self.on = not self.on

    def position(self):
        base = (self.actor.center_world() if self.use_center
                else self.actor.world_pos())
        return tuple(p + o for p, o in zip(base, self.offset))

    def update(self):
        self.actor.emissive = tuple(self.color * 0.6) if self.on else (0, 0, 0)

    def upload(self, shader, i):
        shader.setVec3(f"lights[{i}].position", self.position())
        shader.setVec3(f"lights[{i}].color", self.color)
        shader.setInt(f"lights[{i}].on", int(self.on))
        shader.setInt(f"lights[{i}].env", self.env)