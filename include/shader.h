// Allows for external bindings of OpenGL functions
// SDL2 handles the rest 
#ifdef _WIN32 
    //#include <gl/GL.h>
    #define GL_VERSION_4_4
    #include <GL/glew.h>
    #include <SDL_opengl.h>
#else
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #define GL_VERSION_2_2
    #include <SDL2/SDL_opengl.h>
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

enum ShaderType {
    VERTEX = 0,
    FRAGMENT = 1,
    COMPUTE = 2
};

class Shader {
    private:
        bool validShader = false;

    public:
        unsigned int ID;
        std::string vertexCode = "";
        std::string fragmentCode = "";
        char errorMsg[1024];

        Shader() {

        }

        Shader(std::string _vs, std::string _fs) {
            this->vertexCode = _vs;
            this->fragmentCode = _fs;
            Compile();
        }

        Shader(const char* vertexFilePath, const char* fragmentFilePath) {
            // retrieve the vertex/fragment source code from filePath
            GetShaderFromFile(vertexFilePath, "VERTEX");
            GetShaderFromFile(fragmentFilePath, "FRAGMENT");
            Compile();
        }

    public:
        /*
        IMPLEMENT
        glGetShaderiv
        glGetShaderInfoLog
        glGetAttribLocation
        */
       bool GetShaderFromString(std::string source, std::string type) {
            if (type == "VERTEX") {
                this->vertexCode = source;
            } else {
                this->fragmentCode = source;
            }
            if (this->validShader) {
                Compile();
            }
            return true;
        }

        bool GetShaderFromCharBuffer(char* charBuffer, std::string type) {
            return GetShaderFromString(std::string(charBuffer), type);
        }

        bool GetShaderFromFile(const char* filePath, std::string type) {
            std::string code;
            std::ifstream shaderFile;
            // ensure ifstream objects can throw exceptions:
            shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            try {
                // open files
                shaderFile.open(filePath);
                std::stringstream shaderStream;
                // read file's buffer contents into streams
                shaderStream << shaderFile.rdbuf();
                // close file handlers
                shaderFile.close();
                // convert stream into string
                code = shaderStream.str();
            }
            catch (std::ifstream::failure& e) {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
                return false;
            }
            return GetShaderFromString(code, type);
        }

        bool CheckShaderCompileErrors(unsigned int shader, std::string type) {
            int success;
            char infoLog[1024];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                strcpy(errorMsg, infoLog);
                return false;
            } else {
                strcpy(errorMsg, "OK: Compiled successfully!");
            }
            return true;
        }

        bool CheckProgramCompileErrors(unsigned int program) {
            int success;
            char infoLog[1024];
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(program, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                return false;
            }
            return true;
        }

        bool Compile() {
            const char* vShaderCode = vertexCode.c_str();
            const char * fShaderCode = fragmentCode.c_str();

            unsigned int vertex, fragment;

            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            std::cout << "glCreateShader(" << GL_VERTEX_SHADER << ")" << std::endl;
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            std::cout << "glShaderSource(" << vertex << ", 1, " << &vShaderCode << ", NULL)" << std::endl;
            
            glCompileShader(vertex);
            std::cout << "glCompileShader(" << vertex << ")" << std::endl;
            if (!CheckShaderCompileErrors(vertex, "VERTEX")) {
                return false;
            }

            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            std::cout << "glCreateShader(" << GL_VERTEX_SHADER << ")" << std::endl;
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            std::cout << "glShaderSource(" << fragment << ", 1, " << &fShaderCode << ", NULL)" << std::endl;
            
            glCompileShader(fragment);
            std::cout << "glCompileShader(" << fragment << ")" << std::endl;
            if (!CheckShaderCompileErrors(fragment, "FRAGMENT")) {
                return false;
            }

            // shader Program
            this->ID = glCreateProgram();
            std::cout << "glCreateProgram()" << std::endl;
            glAttachShader(ID, vertex);
            std::cout << "glAttachShader(" << ID << ", " << vertex << ")" << std::endl;
            glAttachShader(ID, fragment);
            std::cout << "glAttachShader(" << ID << ", " << fragment << ")" << std::endl;
            // glBindFragDataLocation(ID, 0, "oColour");
            glLinkProgram(ID);
            std::cout << "glLinkProgram(" << ID << ")" << std::endl;
            if (!CheckProgramCompileErrors(ID)) {
                return false;
            }

            // delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            this->validShader = true;
            return true;
        }

        bool Use() {
            if (this->validShader) {
                glUseProgram(this->ID);
                std::cout <<"glUseProgram(" << ID << ")" << std::endl;
                return true;
            }
            return false;
        }

        void SetBool(const std::string& name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        void SetInt(const std::string& name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        void SetFloat(const std::string& name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }

        void SetVec2(const std::string& name, float x, float y) const {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
        }

        void SetVec3(const std::string& name, float x, float y, float z) const {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        }

        void SetVec4(const std::string& name, float x, float y, float z, float w) const {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        }

};