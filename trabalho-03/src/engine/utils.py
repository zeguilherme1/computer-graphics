import glfw
from OpenGL.GL import *
import numpy as np

def clamp(value, lo, hi):
    return max(lo, min(hi, value))

def key_dir(window, k_neg, k_pos):
    d = 0
    if glfw.get_key(window, k_neg) == glfw.PRESS:
        d -= 1
    if glfw.get_key(window, k_pos) == glfw.PRESS:
        d += 1
    return d

def gl_state_setup():
    glEnable(GL_TEXTURE_2D)
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE)
    glEnable(GL_DEPTH_TEST)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glEnable(GL_LINE_SMOOTH)


def ground_height(positions, x, z, max_y):
    """Retorna a altura do chão abaixo de um ponto ou None"""
    tris = np.asarray(positions, dtype=np.float64).reshape(-1, 3, 3)

    ax, az = tris[:, 0, 0], tris[:, 0, 2]
    bx, bz = tris[:, 1, 0], tris[:, 1, 2]
    cx, cz = tris[:, 2, 0], tris[:, 2, 2]

    den = (bz - cz) * (ax - cx) + (cx - bx) * (az - cz)

    ok = np.abs(den) > 1e-12
    den = np.where(ok, den, 1.0)

    l1 = ((bz - cz) * (x - cx) + (cx - bx) * (z - cz)) / den
    l2 = ((cz - az) * (x - cx) + (ax - cx) * (z - cz)) / den
    l3 = 1.0 - l1 - l2

    inside = (
        ok
        & (l1 >= -1e-6)
        & (l2 >= -1e-6)
        & (l3 >= -1e-6)
    )

    y = l1 * tris[:, 0, 1] + l2 * tris[:, 1, 1] + l3 * tris[:, 2, 1]
    y = y[inside & (y <= max_y)]

    return float(y.max()) if y.size else None
