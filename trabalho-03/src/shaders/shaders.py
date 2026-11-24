
from OpenGL.GL import *


class Shader:
    """Gerencia a compilação e o uso dos shaders"""

    def __init__(self, vertexPath: str, fragmentPath: str):
        try:
            with open(vertexPath, "r") as v_file:
                vertexCode = v_file.read()

            with open(fragmentPath, "r") as f_file:
                fragmentCode = f_file.read()

            vertex = glCreateShader(GL_VERTEX_SHADER)
            glShaderSource(vertex, vertexCode)
            glCompileShader(vertex)
            self.checkCompileErrors(vertex, "VERTEX")
            fragment = glCreateShader(GL_FRAGMENT_SHADER)
            glShaderSource(fragment, fragmentCode)
            glCompileShader(fragment)
            self.checkCompileErrors(fragment, "FRAGMENT")

            self.ID = glCreateProgram()
            glAttachShader(self.ID, vertex)
            glAttachShader(self.ID, fragment)
            glLinkProgram(self.ID)
            self.checkCompileErrors(self.ID, "PROGRAM")

            glDeleteShader(vertex)
            glDeleteShader(fragment)

        except IOError as error:
            raise RuntimeError(
                f"Não foi possível ler os arquivos dos shaders: {error}"
            ) from error

    def getProgram(self):
        return self.ID

    def use(self) -> None:
        glUseProgram(self.ID)

    def setBool(self, name: str, value: bool) -> None:
        glUniform1i(glGetUniformLocation(self.ID, name), int(value))

    def setInt(self, name: str, value: int) -> None:
        glUniform1i(glGetUniformLocation(self.ID, name), value)

    def setFloat(self, name: str, value: float) -> None:
        glUniform1f(glGetUniformLocation(self.ID, name), value)

    def checkCompileErrors(self, shader: int, type: str) -> None:
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
    def setVec3(self, name: str, v) -> None:
        glUniform3f(glGetUniformLocation(self.ID, name), *[float(x) for x in v])

    def setMat4(self, name: str, m) -> None:
        glUniformMatrix4fv(glGetUniformLocation(self.ID, name), 1, GL_TRUE, m)