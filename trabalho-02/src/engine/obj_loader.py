import os
import numpy as np
from OpenGL.GL import *
from PIL import Image


def parse_mtl(mtl_path):
    mats, cur = {}, None
    if not os.path.exists(mtl_path):
        return mats
    for line in open(mtl_path, "r"):
        v = line.split()
        if not v:
            continue
        if v[0] == "newmtl":
            cur = v[1]
            mats[cur] = {"map": None, "kd": (1.0, 1.0, 1.0)}
        elif v[0] == "Kd" and cur:
            mats[cur]["kd"] = tuple(float(x) for x in v[1:4])
        elif v[0] == "map_Kd" and cur:
            mats[cur]["map"] = v[-1]
    return mats


def find_texture(base_dir, name):
    name = os.path.basename(name.replace("\\", "/"))
    for sub in ("", "textures"):
        p = os.path.join(base_dir, sub, name)
        if os.path.exists(p):
            return p
    return None


def load_obj(path, normalize=True):
    base_dir = os.path.dirname(path)
    V, VT = [], []
    by_mat = {}
    mtllib, material = None, None

    for line in open(path, "r"):
        if line.startswith("#"):
            continue
        v = line.split()
        if not v:
            continue

        if v[0] == "v":
            V.append([float(x) for x in v[1:4]])
        elif v[0] == "vt":
            VT.append([float(x) for x in v[1:3]])
        elif v[0] == "mtllib":
            mtllib = v[1]
        elif v[0] in ("usemtl", "usemat"):
            material = v[1]
        elif v[0] == "f":
            idx = []
            for tok in v[1:]:
                w = tok.split("/")
                vi = int(w[0])
                vi = vi - 1 if vi > 0 else len(V) + vi
                ti = None
                if len(w) > 1 and w[1]:
                    t = int(w[1])
                    ti = t - 1 if t > 0 else len(VT) + t
                idx.append((vi, ti))

            lst = by_mat.setdefault(material, [])
            for i in range(1, len(idx) - 1):
                for k in (idx[0], idx[i], idx[i + 1]):
                    pos = V[k[0]]
                    uv = VT[k[1]] if k[1] is not None else [0.0, 0.0]
                    lst.append((pos, uv))

    positions, uvs, groups = [], [], []
    for mat, verts in by_mat.items():
        start = len(positions)
        for pos, uv in verts:
            positions.append(pos)
            uvs.append(uv)
        groups.append((mat, start, len(verts)))

    positions = np.array(positions, dtype=np.float32)
    uvs = np.array(uvs, dtype=np.float32)

    if normalize and len(positions):
        mn, mx = positions.min(axis=0), positions.max(axis=0)
        center = (mn + mx) / 2.0
        size = (mx - mn).max()
        positions = (positions - center) / size

    materials = {}
    if mtllib:
        materials = parse_mtl(os.path.join(base_dir, mtllib))

    tex_paths = {}
    for mat, _, _ in groups:
        m = materials.get(mat)
        if m and m["map"]:
            p = find_texture(base_dir, m["map"])
            tex_paths[mat] = p if p else m["kd"]
        elif m:
            tex_paths[mat] = m["kd"]
        else:
            tex_paths[mat] = None

    return positions, uvs, groups, tex_paths


def load_texture(path, flip=True):
    """path: caminho de imagem, tupla (r,g,b) ou None (branco).
    flip=True  -> modelos vindos do Blender
    flip=False -> modelos gerados pelo glb_to_obj.py"""
    tex = glGenTextures(1)
    glBindTexture(GL_TEXTURE_2D, tex)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1)

    if path is None:
        path = (1.0, 1.0, 1.0)
    if isinstance(path, tuple):                    # cor sólida -> textura 1x1
        rgba = bytes([int(max(0.0, min(1.0, c)) * 255) for c in path] + [255])
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, rgba)
        return tex

    img = Image.open(path).convert("RGBA")
    data = img.tobytes("raw", "RGBA", 0, -1 if flip else 1)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data)
    glGenerateMipmap(GL_TEXTURE_2D)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    return tex