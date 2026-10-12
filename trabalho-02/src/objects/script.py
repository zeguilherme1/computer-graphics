# uso: python glb_to_obj.py castelo.glb objects/peach_v2
import os, sys
import numpy as np
import trimesh

src, out = sys.argv[1], sys.argv[2]
os.makedirs(out, exist_ok=True)

scene = trimesh.load(src, force="scene")
meshes = scene.dump()          # já aplica as transformações da cena

tex_files = {}                 # id(imagem) -> nome do arquivo
materials = {}                 # (arquivo, cor) -> nome do material
obj = ["mtllib castle.mtl"]
v_off = 0

for i, m in enumerate(meshes):
    mat = getattr(m.visual, "material", None)
    img = getattr(mat, "baseColorTexture", None)
    factor = getattr(mat, "baseColorFactor", None)
    color = tuple(round(float(c) / 255.0, 4) for c in factor[:3]) if factor is not None else (0.8, 0.8, 0.8)

    fname = None
    if img is not None:
        if id(img) not in tex_files:
            fname = f"tex_{len(tex_files)}.png"
            img.convert("RGBA").save(os.path.join(out, fname))
            tex_files[id(img)] = fname
        fname = tex_files[id(img)]

    key = (fname, color if fname is None else None)
    if key not in materials:
        materials[key] = f"mat_{len(materials)}"
    name = materials[key]

    verts = np.asarray(m.vertices)
    uv = getattr(m.visual, "uv", None)
    uv = np.zeros((len(verts), 2)) if uv is None else np.asarray(uv)

    obj.append(f"o mesh_{i}")
    obj += [f"v {x:.6f} {y:.6f} {z:.6f}" for x, y, z in verts]
    obj += [f"vt {u:.6f} {1.0 - v:.6f}" for u, v in uv]   # glTF -> convenção OBJ
    obj.append(f"usemtl {name}")
    for a, b, c in np.asarray(m.faces) + v_off + 1:
        obj.append(f"f {a}/{a} {b}/{b} {c}/{c}")
    v_off += len(verts)

with open(os.path.join(out, "castle.obj"), "w") as f:
    f.write("\n".join(obj) + "\n")

with open(os.path.join(out, "castle.mtl"), "w") as f:
    for (fname, color), name in materials.items():
        f.write(f"newmtl {name}\n")
        if fname:
            f.write(f"Kd 1 1 1\nmap_Kd {fname}\n\n")
        else:
            f.write(f"Kd {color[0]} {color[1]} {color[2]}\n\n")

print("meshes:", len(meshes), "| materiais:", len(materials), "| texturas:", len(tex_files))
