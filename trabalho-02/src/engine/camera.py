import math
import numpy as np
import glfw

def normalize(v):
    n = np.linalg.norm(v)
    return v / n if n > 0 else v

def look_at(eye, center, up):
    f = normalize(center - eye)
    s = normalize(np.cross(f, up))
    u = np.cross(s, f)
    m = np.identity(4, dtype=np.float32)
    m[0, :3], m[1, :3], m[2, :3] = s, u, -f
    m[0, 3] = -np.dot(s, eye)
    m[1, 3] = -np.dot(u, eye)
    m[2, 3] = np.dot(f, eye)
    return m

def perspective(fov_deg, aspect, near=0.3, far=300.0):
    f = 1.0 / math.tan(math.radians(fov_deg) / 2.0)
    m = np.zeros((4, 4), dtype=np.float32)
    m[0, 0] = f / aspect
    m[1, 1] = f
    m[2, 2] = (far + near) / (near - far)
    m[2, 3] = 2 * far * near / (near - far)
    m[3, 2] = -1.0
    return m


def model(angle_deg=0.0, scale=1.0, translate=(0.0, 0.0, 0.0)):
    a = math.radians(angle_deg)
    c, s = math.cos(a), math.sin(a)
    m = np.identity(4, dtype=np.float32)
    m[0, 0], m[0, 2] = c * scale, s * scale
    m[2, 0], m[2, 2] = -s * scale, c * scale
    m[1, 1] = scale
    m[:3, 3] = translate
    return m

class Camera:
    def __init__(self, position=(0.0, 0.6, 4.0), yaw=-90.0, pitch=0.0):
        self.pos = np.array(position, dtype=np.float32)
        self.yaw, self.pitch = yaw, pitch
        self.fov = 60.0
        self.speed = 2
        self.sensitivity = 0.1
        self.up = np.array([0.0, 1.0, 0.0], dtype=np.float32)
        self._last = None
        self._update_vectors()

    def _update_vectors(self):
        y, p = math.radians(self.yaw), math.radians(self.pitch)
        self.front = normalize(np.array([
            math.cos(y) * math.cos(p),
            math.sin(p),
            math.sin(y) * math.cos(p)], dtype=np.float32))
        self.right = normalize(np.cross(self.front, self.up))

    def on_mouse(self, window, x, y):
        if self._last is None:
            self._last = (x, y)
        dx, dy = x - self._last[0], self._last[1] - y
        self._last = (x, y)
        self.yaw += dx * self.sensitivity
        self.pitch = max(-89.0, min(89.0, self.pitch + dy * self.sensitivity))
        self._update_vectors()

    def on_scroll(self, window, dx, dy):
        self.fov = max(20.0, min(90.0, self.fov - dy * 2.0))

    def process_keyboard(self, window, dt):
        v = self.speed * dt
        if glfw.get_key(window, glfw.KEY_W) == glfw.PRESS: self.pos += self.front * v
        if glfw.get_key(window, glfw.KEY_S) == glfw.PRESS: self.pos -= self.front * v
        if glfw.get_key(window, glfw.KEY_D) == glfw.PRESS: self.pos += self.right * v
        if glfw.get_key(window, glfw.KEY_A) == glfw.PRESS: self.pos -= self.right * v
        if glfw.get_key(window, glfw.KEY_SPACE) == glfw.PRESS: self.pos += self.up * v
        if glfw.get_key(window, glfw.KEY_LEFT_SHIFT) == glfw.PRESS: self.pos -= self.up * v

    def view_matrix(self):
        return look_at(self.pos, self.pos + self.front, self.up)

    def projection_matrix(self, aspect):
        return perspective(self.fov, aspect)