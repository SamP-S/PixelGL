// Allows for external bindings of OpenGL functions
// SDL2 handles the rest 

#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL_opengl.h>

// includes
#include <map>
#include <assert.h>
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <memory>

#define LA_OPEN_GL
#include "la.h"

#include "window.h"
#include "frametimer.h"

enum TextureType {
    NONE = 0,
    DIFFUSE = 1
};

enum PropertyType {
    FLOAT = 0,
    DOUBLE = 1,
    STRING = 2,
    INT = 3,
    UINT = 4,
    BUFFER = 5,
    TEXTURE = 6
};

class MaterialPropertyFrame {
    private: 
        std::string mName;
        PropertyType mType;
        TextureType mTexType;

    public:
        MaterialPropertyFrame(std::string s, PropertyType p, TextureType t) {
            this->mName = s;
            this->mType = p;
            this->mTexType = t;
        }

        std::string GetName() {
            return this->mName;
        }

        bool SetName(std::string s) {
            this->mName = s;
            return true;
        }

        PropertyType GetType() {
            return this->mType;
        }

        TextureType GetTexType() {
            return this->mTexType;
        }
};

template<typename T>
class MaterialProperty : public MaterialPropertyFrame {
    private:
        T data;
};


class Material {
    private:
        unsigned int mNumProperties = 0;
        std::vector<std::shared_ptr<MaterialPropertyFrame>> mProperties;
   
    public: 
        bool Add(MaterialProperty<float>& p) {
            std::shared_ptr<MaterialPropertyFrame> ptr(&p);
            this->mProperties.push_back(ptr);
            this->mNumProperties += 1;
            return true;
        }

        bool Get(int index, MaterialProperty<float>* mp) {
            if (index < 0 || index >= static_cast<int>(this->mNumProperties)) {
                return false;
            } else {
                if (this->mProperties[index]->GetType() == PropertyType::FLOAT) {
                    mp = static_cast<MaterialProperty<float>*>(this->mProperties[index].get());
                } else {
                    mp = NULL;
                    return false;
                }
                return true;
            }
        }
};

class Face {
    private:
        unsigned int mNumIndices;
        unsigned int mIndices[];
};

class Mesh {
    private:
        std::string mName;
        unsigned int mNumVertices;
        unsigned int mNumNormals;
        unsigned int mNumTexCoords;
        unsigned int mNumFaces;
        LA::vec3* mVertices;
        LA::vec3* mNormals;
        LA::vec3* mTextureCoords;
        LA::vec4* mColours;
        std::vector<Face> mFaces;
        Material mMaterial;

    public:
        ~Mesh() {
            delete mVertices;
            delete mNormals;
            delete mTextureCoords;
            delete mColours;
        }
};

enum Tex_Filtering {
    NEAREST = 0,
    LINEAR = 1,
    NEAREST_MIPMAP_NEAREST = 2,
    LINEAR_MIPMAP_NEAREST = 3,
    NEAREST_MIPMAP_LINEAR = 4,
    LINEAR_MIPMAP_LINEAR = 5
};

enum Tex_Wrapping {
    REPEAT = 0,
    MIRRORED_REPEAT = 1,
    CLAMP_TO_EDGE = 2,
    CLAMP_TO_BORDER = 3
};

enum Tex_Params {
    TEXTURE_MIN_FILTER = 0,
    TEXTURE_MAG_FILTER = 1,
    TEXTURE_WRAP_S = 2,
    TEXTURE_WRAP_T = 3,
    TEXTURE_WRAP_R = 4
};

enum Target {
    TEXTURE_1D = 0,
    TEXTURE_2D = 1,
    TEXTURE_3D = 2,
    TEXTURE_RECTANGLE = 3,
    TEXTURE_BUFFER = 4,
    TEXTURE_CUBE_MAP = 5,
    TEXTURE_2D_MULTISAMPLE = 6
};

enum Comparison {
    NEVER = 0,
    ALWAYS = 1,
    LESS = 2,
    LEQUAL = 3,
    EQUAL = 4,
    NOT_EQUAL = 5,
    GEQUAL = 6,
    GREATER = 7
};

class Texture {
    private:
        unsigned int mTextureID;
        std::string mName;
        std::string mFilePath;
        unsigned int mHeight;
        unsigned int mWidth;
        unsigned char* data;
        Tex_Wrapping mWrappingS = Tex_Wrapping::REPEAT;
        Tex_Wrapping mWrappingT = Tex_Wrapping::REPEAT;
        Tex_Wrapping mWrappingR = Tex_Wrapping::REPEAT;
        Tex_Filtering mFilteringMin = Tex_Filtering::NEAREST_MIPMAP_LINEAR;
        Tex_Filtering mFilteringMag = Tex_Filtering::LINEAR;
        Target mTarget;

    public:
        ~Texture() {
            delete data;
        }


};

// class ResoureManager {
//     private:
//     std::vector<GLuint*> vbos;
//     std::vector<GLuint*> vaos;
//     std::vector<GLuint*> textures;

//     std::map<std::string, GLuint> vbo_map;
//     std::map<std::string, GLuint> vao_map;
//     std::map<std::string, GLuint> texture_map;
    
//     ResoureManager() {

//     }

//     bool Get() {

//     }

//     bool LoadModel() {
//         return true;
//     }

//     bool LoadTexure() {
//         return true;
//     }
// };

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

        Shader() {

        }

        Shader(std::string vs, std::string fs) {
            this->vertexCode = vs;
            this->fragmentCode = fs;
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
                return false;
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
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            if (!CheckShaderCompileErrors(vertex, "VERTEX")) {
                return false;
            }

            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            if (!CheckShaderCompileErrors(fragment, "FRAGMENT")) {
                return false;
            }

            // shader Program
            this->ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            // glBindFragDataLocation(ID, 0, "oColour");
            glLinkProgram(ID);
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

        void SetVec2(const std::string& name, const LA::vec2& value) const {
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }

        void SetVec2(const std::string& name, float x, float y) const {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
        }

        void SetVec3(const std::string& name, const LA::vec3& value) const {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }

        void SetVec3(const std::string& name, float x, float y, float z) const {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        }

        void SetVec4(const std::string& name, const LA::vec4& value) const {
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }

        void SetVec4(const std::string& name, float x, float y, float z, float w) const {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        }

        void SetMat2(const std::string& name, const LA::mat2& value) const {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
        }

        void SetMat3(const std::string& name, const LA::mat3& value) const {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
        }

        void SetMat4(const std::string& name, const LA::mat4& value) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
        }
};

class GraphicsEngine {
    private:
        unsigned int frameNum = 0;

    public:
        void* context;
        WindowManager* window;
        FrameTimer ft;
        Shader shader;
        GLuint vbo, vao, fbo, texColour, texDepthStencil;
        int width, height;


        GraphicsEngine(WindowManager* window) {
            AttachWindow(window);
            SetViewport(window->width, window->height);
            InitGL();
            ft = FrameTimer();
        }

        ~GraphicsEngine() {
            // delete context; delete makes error even though pointer?!?
            // delete window; causesd unknown signal error?
        }

        bool AttachWindow(WindowManager* window) {
            if (window == NULL) {
                return false;
            }
            this->window = window;
            return true;
        }

        bool DetachWindow() {
            this->window = NULL;
            return true;
        }

        void InitGL() {
            // Initialise GLEW
            glewExperimental = GL_TRUE;
            glewInit();

            float vertices[] = {
                -1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f
            };
            
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
                std::cout << "COOL: FBO hasn't yabber dabber died yet" << std::endl;

            glGenTextures(1, &texColour);
            
            glBindTexture(GL_TEXTURE_2D, texColour);
            glTexImage2D(   GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                                    GL_TEXTURE_2D, texColour, 0);

            glGenTextures(1, &texDepthStencil);
            glBindTexture(GL_TEXTURE_2D, texDepthStencil);
            glTexImage2D(   GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, 
                            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
            );

            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
                                    GL_TEXTURE_2D, texDepthStencil, 0);

            GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if(status != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            }
                
            glBindFramebuffer(GL_FRAMEBUFFER, 0); 


            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            shader = Shader("shaders/base.vs", "shaders/base.fs");
            glBindFragDataLocation(shader.ID, 0, "oColour");
            shader.Use();

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            GLint posAttrib = glGetAttribLocation(shader.ID, "iPosition");
            glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(posAttrib);
        }
        
        // function to reset our viewport after a window resize
        int SetViewport(int _width, int _height) {
            this->width = _width;
            this->height = _height;

            /* Protect against a divide by zero */
            if (height == 0 ) {
                height = 1;
            }
            float ratio = width / height;

            return 1;
        }

        void Render() {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
            /* Clear The Screen And The Depth Buffer */
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glBindVertexArray(vao);
            glDisable(GL_DEPTH_TEST);
            shader.Use();

            shader.SetVec3("iResolution", window->width, window->height, 1.0f);
            shader.SetFloat("iTime", ft.GetTotalElapsed());
            shader.SetFloat("iTimeDelta", ft.GetFrameElapsed());
            shader.SetInt("iFrame", frameNum);
                     
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            ft.Frame();
            frameNum += 1;
        }

        static void ErrorCheck() {
            GLenum error = glGetError();

            if (error == GL_NO_ERROR) {
                std::cout << "OK: all good" << std::endl;
            } else {
                std::cout << "ERROR: ";
            }

            if (error == GL_INVALID_OPERATION) {
                std::cout << "INVALID OPERATION" << std::endl;
            } else if (error == GL_INVALID_VALUE) {
                std::cout << "NO VALUE" << std::endl;
            } else if (error == GL_OUT_OF_MEMORY) {
                std::cout << "NO MEMORY" << std::endl;
            } else if (error == GL_STACK_OVERFLOW) {
                std::cout << "NO OVERFLOW" << std::endl;
            } else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
                std::cout << "Invalid Framebuffer" << std::endl;
            } else if (error == GL_OUT_OF_MEMORY) {
                std::cout << "Out of Memory" << std::endl;
            }
        }

};