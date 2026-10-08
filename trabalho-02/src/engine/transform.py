import glm
import numpy as np

def model_matrix(angle_y, scale=1.0):
    m = glm.mat4(1.0)
    m = glm.rotate(m, angle_y, glm.vec3(0, 1, 0))
    m = glm.scale(m, glm.vec3(scale))
    return m

def view_matrix(distance=5.0):
    return glm.lookAt(glm.vec3(0, 0, distance),
                      glm.vec3(0, 0, 0),          
                      glm.vec3(0, 1, 0))          

def projection_matrix(width, height):
    return glm.perspective(glm.radians(45.0), width / height, 0.1, 100.0)

def to_np(m):
    return np.array(m, dtype=np.float32)