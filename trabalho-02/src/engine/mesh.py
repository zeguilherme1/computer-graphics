import ctypes
import numpy as np
from OpenGL.GL import *
from engine import obj_loader as ld


class Mesh:
    def __init__(self, program, obj_path, override_texture=None, flip=True):
        positions, uvs, groups, tex_paths = ld.load_obj(obj_path)
        print(f"{obj_path}: {len(positions)} vértices, {len(groups)} materiais")

        if not override_texture:
            sem_textura = [m for m, p in tex_paths.items()
                           if p is None or isinstance(p, tuple)]
            print(f"materiais sem imagem: {len(sem_textura)}", sem_textura[:10])

        self.groups = []
        cache = {}
        for mat, start, count in groups:
            path = override_texture or tex_paths[mat]
            if path not in cache:
                cache[path] = ld.load_texture(path, flip)
            self.groups.append((cache[path], start, count))

        self.vbos = glGenBuffers(2)
        self.attribs = [
            (self.vbos[0], glGetAttribLocation(program, "position"), 3),
            (self.vbos[1], glGetAttribLocation(program, "texture_coord"), 2),
        ]
        for (vbo, _, _), data in zip(self.attribs, (positions, uvs)):
            glBindBuffer(GL_ARRAY_BUFFER, vbo)
            glBufferData(GL_ARRAY_BUFFER, data.nbytes, data, GL_STATIC_DRAW)

    def draw(self):
        for vbo, loc, size in self.attribs:
            if loc < 0:
                continue
            glBindBuffer(GL_ARRAY_BUFFER, vbo)
            glEnableVertexAttribArray(loc)
            glVertexAttribPointer(loc, size, GL_FLOAT, False, 0, ctypes.c_void_p(0))

        glActiveTexture(GL_TEXTURE0)
        for tex, start, count in self.groups:
            glBindTexture(GL_TEXTURE_2D, tex)
            glDrawArrays(GL_TRIANGLES, start, count)