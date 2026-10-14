import glfw

def clamp(value, lo, hi):
    # Mantém o valor dentro do intervalo permitido
    return max(lo, min(hi, value))

def key_dir(window, k_neg, k_pos):
    # Retorna a direção indicada pelas teclas pressionadas
    d = 0
    if glfw.get_key(window, k_neg) == glfw.PRESS:
        d -= 1
    if glfw.get_key(window, k_pos) == glfw.PRESS:
        d += 1
    return d