import numpy as np


def ground_height(positions, x, z, max_y):
    """Retorna a altura do chão abaixo de um ponto ou None"""

    # Organiza os vértices em grupos de três
    tris = np.asarray(positions, dtype=np.float64).reshape(-1, 3, 3)

    ax, az = tris[:, 0, 0], tris[:, 0, 2]
    bx, bz = tris[:, 1, 0], tris[:, 1, 2]
    cx, cz = tris[:, 2, 0], tris[:, 2, 2]

    # Calcula os denominadores das coordenadas baricêntricas
    den = (bz - cz) * (ax - cx) + (cx - bx) * (az - cz)

    # Ignora triângulos degenerados
    ok = np.abs(den) > 1e-12
    den = np.where(ok, den, 1.0)

    l1 = ((bz - cz) * (x - cx) + (cx - bx) * (z - cz)) / den
    l2 = ((cz - az) * (x - cx) + (ax - cx) * (z - cz)) / den
    l3 = 1.0 - l1 - l2

    # Seleciona os triângulos que contêm o ponto
    inside = (
        ok
        & (l1 >= -1e-6)
        & (l2 >= -1e-6)
        & (l3 >= -1e-6)
    )

    # Interpola a altura do ponto em cada triângulo
    y = l1 * tris[:, 0, 1] + l2 * tris[:, 1, 1] + l3 * tris[:, 2, 1]

    # Considera apenas as alturas abaixo do limite informado
    y = y[inside & (y <= max_y)]

    return float(y.max()) if y.size else None
