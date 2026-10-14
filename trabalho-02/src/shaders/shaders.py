
from OpenGL.GL import *


class Shader:
    """Gerencia a compilação e o uso dos shaders"""

    def __init__(self, vertexPath: str, fragmentPath: str):
        try:
            # Lê os códigos dos shaders
            with open(vertexPath, "r") as v_file:
                vertexCode = v_file.read()

            with open(fragmentPath, "r") as f_file:
                fragmentCode = f_file.read()

            # Compila o vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER)
            glShaderSource(vertex, vertexCode)
            glCompileShader(vertex)
            self.checkCompileErrors(vertex, "VERTEX")

            # Compila o fragment shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER)
            glShaderSource(fragment, fragmentCode)
            glCompileShader(fragment)
            self.checkCompileErrors(fragment, "FRAGMENT")

            # Vincula os shaders em um programa
            self.ID = glCreateProgram()
            glAttachShader(self.ID, vertex)
            glAttachShader(self.ID, fragment)
            glLinkProgram(self.ID)
            self.checkCompileErrors(self.ID, "PROGRAM")

            # Libera os shaders após a vinculação
            glDeleteShader(vertex)
            glDeleteShader(fragment)

        except IOError as error:
            raise RuntimeError(
                f"Não foi possível ler os arquivos dos shaders: {error}"
            ) from error

    def getProgram(self):
        return self.ID

    def use(self) -> None:
        # Ativa o programa de shaders
        glUseProgram(self.ID)

    def setBool(self, name: str, value: bool) -> None:
        glUniform1i(glGetUniformLocation(self.ID, name), int(value))

    def setInt(self, name: str, value: int) -> None:
        glUniform1i(glGetUniformLocation(self.ID, name), value)

    def setFloat(self, name: str, value: float) -> None:
        glUniform1f(glGetUniformLocation(self.ID, name), value)

    def checkCompileErrors(self, shader: int, type: str) -> None:
        # Verifica erros de compilação ou vinculação
        if type != "PROGRAM":
            success = glGetShaderiv(shader, GL_COMPILE_STATUS)

            if not success:
                infoLog = glGetShaderInfoLog(shader)
                print(
                    f"ERROR::SHADER_COMPILATION_ERROR of type: {type}\n"
                    f"{infoLog.decode()}"
                )
        else:
            success = glGetProgramiv(shader, GL_LINK_STATUS)

            if not success:
                infoLog = glGetProgramInfoLog(shader)
                print(
                    f"ERROR::PROGRAM_LINKING_ERROR of type: {type}\n"
                    f"{infoLog.decode()}"
                )
