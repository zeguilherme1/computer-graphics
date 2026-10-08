import ctypes
import numpy as np
from OpenGL.GL import *
from engine import obj_loader as ld

class Mesh:
    def __init__(self, program, obj_path, texture_paths):
        ld.load_model_from_file(obj_path)
        self.first, self.count = ld.load_obj_and_texture(obj_path, texture_paths)
        self._upload(program)

    def _upload(self, program):
        self.vbos = glGenBuffers(2)

        verts = np.zeros(len(ld.vertices_list), [("position", np.float32, 3)])
        verts["position"] = ld.vertices_list
        self._attrib(program, self.vbos[0], verts, "position", 3)

        tex = np.zeros(len(ld.textures_coord_list), [("position", np.float32, 2)])
        tex["position"] = ld.textures_coord_list
        self._attrib(program, self.vbos[1], tex, "texture_coord", 2)

    @staticmethod
    def _attrib(program, vbo, data, name, size):
        glBindBuffer(GL_ARRAY_BUFFER, vbo)
        glBufferData(GL_ARRAY_BUFFER, data.nbytes, data, GL_STATIC_DRAW)
        loc = glGetAttribLocation(program, name)
        glEnableVertexAttribArray(loc)
        glVertexAttribPointer(loc, size, GL_FLOAT, False,
                              data.strides[0], ctypes.c_void_p(0))

    def draw(self):
        glBindTexture(GL_TEXTURE_2D, 0)
        glDrawArrays(GL_TRIANGLES, self.first, self.count)