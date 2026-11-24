import ctypes
import numpy as np
from OpenGL.GL import *
from engine import obj_loader as ld


class Mesh:
    """Gerencia os dados e a renderização de uma malha"""

    def __init__(self, program, obj_path, override_texture=None, flip=True):
        positions, uvs, groups, tex_paths = ld.load_obj(obj_path)
        pts = np.asarray(positions, dtype=np.float32).reshape(-1, 3)
        self.center = (pts.min(axis=0) + pts.max(axis=0)) / 2.0

        self.groups = []
        cache = {}

        # Reutiliza texturas que apontam para o mesmo caminho
        for mat, start, count in groups:
            path = override_texture or tex_paths[mat]

            if path not in cache:
                cache[path] = ld.load_texture(path, flip)

            self.groups.append((cache[path], start, count))

        # Normais por face
        normals = self._face_normals(positions)

        # Cria buffers para posições, coordenadas de textura e normais
        self.vbos = glGenBuffers(3)

        self.attribs = [
            (self.vbos[0], glGetAttribLocation(program, "position"), 3),
            (self.vbos[1], glGetAttribLocation(program, "texture_coord"), 2),
            (self.vbos[2], glGetAttribLocation(program, "normal"), 3),
        ]

        for (vbo, _, _), data in zip(self.attribs, (positions, uvs, normals)):
            glBindBuffer(GL_ARRAY_BUFFER, vbo)
            glBufferData(GL_ARRAY_BUFFER, data.nbytes, data, GL_STATIC_DRAW)

    @staticmethod
    def _face_normals(positions):
        """Calcula a normal de cada triângulo e a repete nos 3 vértices"""
        tri = np.asarray(positions, dtype=np.float32).reshape(-1, 3, 3)
        n = np.cross(tri[:, 1] - tri[:, 0], tri[:, 2] - tri[:, 0])
        length = np.linalg.norm(n, axis=1, keepdims=True)
        n = n / np.maximum(length, 1e-8)
        return np.repeat(n, 3, axis=0).astype(np.float32)

    def draw(self):
        """Desenha os grupos da malha com suas respectivas texturas"""
        for vbo, loc, size in self.attribs:
            if loc < 0:
                continue

            glBindBuffer(GL_ARRAY_BUFFER, vbo)
            glEnableVertexAttribArray(loc)
            glVertexAttribPointer(
                loc, size, GL_FLOAT, False, 0, ctypes.c_void_p(0)
            )

        glActiveTexture(GL_TEXTURE0)

        # desenha cada grupo usando sua textura
        for tex, start, count in self.groups:
            glBindTexture(GL_TEXTURE_2D, tex)
            glDrawArrays(GL_TRIANGLES, start, count)