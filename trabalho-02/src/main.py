import glfw
from OpenGL.GL import *
import numpy as np
import glm
import math
from numpy import random
from PIL import Image

from shaders.shader_s import Shader


glfw.init()
glfw.window_hint(glfw.VISIBLE, glfw.FALSE)
window = glfw.create_window(800, 600, "Programa", None, None)

if (window == None):
    print("Failed to create GLFW window")
    glfwTerminate()

glfw.make_context_current(window)


ourShader = Shader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs")
ourShader.use()

program = ourShader.getProgram()


glEnable(GL_TEXTURE_2D)
glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE)
glEnable(GL_BLEND)
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
glEnable(GL_LINE_SMOOTH)


global vertices_list
vertices_list = []
global textures_coord_list
textures_coord_list = []


def load_model_from_file(filename):
    objects = {}
    vertices = []
    texture_coords = []
    faces = []

    material = None

    for line in open(filename, "r"):
        if line.startswith('#'): continue
        values = line.split()
        if not values: continue

        if values[0] == 'v':
            vertices.append(values[1:4])

        elif values[0] == 'vt':
            texture_coords.append(values[1:3])

        elif values[0] in ('usemtl', 'usemat'):
            material = values[1]
        elif values[0] == 'f':
            face = []
            face_texture = []
            for v in values[1:]:
                w = v.split('/')
                face.append(int(w[0]))
                if len(w) >= 2 and len(w[1]) > 0:
                    face_texture.append(int(w[1]))
                else:
                    face_texture.append(0)

            faces.append((face, face_texture, material))

    model = {}
    model['vertices'] = vertices
    model['texture'] = texture_coords
    model['faces'] = faces

    return model


def load_texture_from_file(texture_id, img_textura):
    glBindTexture(GL_TEXTURE_2D, texture_id)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    img = Image.open(img_textura)
    img_width = img.size[0]
    img_height = img.size[1]
    image_data = img.tobytes("raw", "RGB", 0, -1)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data)


def circular_sliding_window_of_three(arr):
    if len(arr) == 3:
        return arr
    circular_arr = arr + [arr[0]]
    result = []
    for i in range(len(circular_arr) - 2):
        result.extend(circular_arr[i:i+3])
    return result


def load_obj_and_texture(objFile, texturesList):
    modelo = load_model_from_file(objFile)

    initial_vertex = len(vertices_list)
    print('Processando modelo {}. Vertice inicial: {}'.format(objFile, len(vertices_list)))
    faces_visited = []
    for face in modelo['faces']:
        if face[2] not in faces_visited:
            faces_visited.append(face[2])
        for vertice_id in circular_sliding_window_of_three(face[0]):
            vertices_list.append(modelo['vertices'][vertice_id - 1])
        for texture_id in circular_sliding_window_of_three(face[1]):
            textures_coord_list.append(modelo['texture'][texture_id - 1])

    final_vertex = len(vertices_list)
    print('Processando modelo {}. Vertice final: {}'.format(objFile, len(vertices_list)))

    for id in range(len(texturesList)):
        load_texture_from_file(id, texturesList[id])

    return initial_vertex, final_vertex - initial_vertex


load_model_from_file('objects/patrick/patrick.obj')


initial_vertex, vertex_size = load_obj_and_texture('objects/patrick/patrick.obj', ['objects/patrick/patrick.png'])

def render_object():
    glBindTexture(GL_TEXTURE_2D, 0)
    glDrawArrays(GL_TRIANGLES, initial_vertex, vertex_size)


buffer_VBO = glGenBuffers(2)


vertices = np.zeros(len(vertices_list), [("position", np.float32, 3)])
vertices['position'] = vertices_list


glBindBuffer(GL_ARRAY_BUFFER, buffer_VBO[0])
glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices, GL_STATIC_DRAW)
stride = vertices.strides[0]
offset = ctypes.c_void_p(0)
loc_vertices = glGetAttribLocation(program, "position")
glEnableVertexAttribArray(loc_vertices)
glVertexAttribPointer(loc_vertices, 3, GL_FLOAT, False, stride, offset)


textures = np.zeros(len(textures_coord_list), [("position", np.float32, 2)])
textures['position'] = textures_coord_list


glBindBuffer(GL_ARRAY_BUFFER, buffer_VBO[1])
glBufferData(GL_ARRAY_BUFFER, textures.nbytes, textures, GL_STATIC_DRAW)
stride = textures.strides[0]
offset = ctypes.c_void_p(0)
loc_texture_coord = glGetAttribLocation(program, "texture_coord")
glEnableVertexAttribArray(loc_texture_coord)
glVertexAttribPointer(loc_texture_coord, 2, GL_FLOAT, False, stride, offset)


glfw.show_window(window)


glEnable(GL_DEPTH_TEST)

def multiplica_matriz(a, b):
    m_a = a.reshape(4,4)
    m_b = b.reshape(4,4)
    m_c = np.dot(m_a, m_b)
    c = m_c.reshape(1,16)
    return c


rotation_angle = 0
rotation_velocity = 0.03

while not glfw.window_should_close(window):
    glfw.poll_events()

    if glfw.get_key(window, glfw.KEY_ESCAPE) == glfw.PRESS:
        window.close()

    if glfw.get_key(window, glfw.KEY_RIGHT) == glfw.PRESS:
        rotation_angle += rotation_velocity

    if glfw.get_key(window, glfw.KEY_LEFT) == glfw.PRESS:
        rotation_angle -= rotation_velocity

    cos_d = math.cos(rotation_angle)
    sin_d = math.sin(rotation_angle)

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glClearColor(1.0, 1.0, 1.0, 1.0)

    mat_rotation_y = np.array([     cos_d,  0.0, sin_d, 0.0,
                                    0.0,    1.0,   0.0, 0.0,
                                    -sin_d, 0.0, cos_d, 0.0,
                                    0.0,    0.0,   0.0, 1.0], np.float32)

    mat_scale       = np.array([    0.025, 0.0, 0.0, 0.0,
                                    0.0, 0.025, 0.0, 0.0,
                                    0.0, 0.0, 0.1, 0.0,
                                    0.0, 0.0, 0.0, 1.0], np.float32)

    mat_transform = multiplica_matriz(mat_scale, mat_rotation_y)

    loc_mat_transform = glGetUniformLocation(program, "mat_transform")
    glUniformMatrix4fv(loc_mat_transform, 1, GL_TRUE, mat_transform)

    render_object()

    glfw.swap_buffers(window)

glfw.terminate()