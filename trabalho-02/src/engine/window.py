import glfw

def initWindow(): 
    glfw.init()
    glfw.window_hint(glfw.VISIBLE, glfw.FALSE)
    window = glfw.create_window(800, 600, "Programa", None, None)
    
    if (window == None):
        print("Failed to create GLFW window")
        glfwTerminate()
    
    glfw.make_context_current(window)
    return window