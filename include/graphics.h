// Allows for external bindings of OpenGL functions
// SDL2 handles the rest





#ifdef _WIN32 
    //#include <gl/GL.h>
    #define GL_VERSION_4_4
    #include <GL/glew.h>
    #include <SDL2/SDL_opengl.h>
#else
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #define GL_VERSION_2_2
    #include <SDL2/SDL_opengl.h>
#endif

#include <string>
#include <map>
#include <assert.h>
#include "m3d.h"
#include "window.h"

/*
class ResoureManager {
    private:
    GLuint vbos[64];
    GLuint vaos[8];
    GLuint textures[64];

    std::map<std::string, GLuint> vbo_map;
    std::map<std::string, GLuint> vao_map;
    std::map<std::string, GLuint> texture_map;
    
    ResoureManager() {

    }

    bool LoadModel() {
        return true;
    }

    bool LoadTexure() {
        return true;
    }
};
*/

class GraphicsEngine 
{
    public:
    void* context;
    WindowManager* window;

    GraphicsEngine() 
    {
        
    }

    bool AttachWindow(WindowManager* window) {
        if (this->window == NULL) {
            return false;
        }
        this->window = window;
        InitGL();
        SetViewport(800, 600);
        Render();
        return true;
    }

    bool DetachWindow() {
        this->window = NULL;
        return true;
    }

    void InitGL()
    {
        /* Initialise GLEW */
        glewExperimental = GL_TRUE;
        glewInit();

        /* Enable smooth shading */
        glShadeModel( GL_SMOOTH );

        /* Set the background black */
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

        /* Depth buffer setup */
        glClearDepth( 1.0f );

        /* Enables Depth Testing */
        glEnable( GL_DEPTH_TEST );

        /* The Type Of Depth Test To Do */
        glDepthFunc( GL_LEQUAL );

        /* Really Nice Perspective Calculations */
        glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    }
    /* function to reset our viewport after a window resize */
    int SetViewport( int width, int height )
    {
        /* Height / width ration */
        GLfloat ratio;

        /* Protect against a divide by zero */
        if ( height == 0 ) {
            height = 1;
        }

        ratio = ( GLfloat )width / ( GLfloat )height;

        /* Setup our viewport. */
        glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

        return 1;
    }

    void Render()
    {
        /* Set the background black */
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        /* Clear The Screen And The Depth Buffer */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        float vertices[] = {
            0.0f,  0.5f, // Vertex 1 (X, Y)
            0.5f, -0.5f, // Vertex 2 (X, Y)
            -0.5f, -0.5f  // Vertex 3 (X, Y)
        };

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        const char* vertexSource = R"glsl(
            #version 150 core

            in vec2 position;

            void main()
            {
                gl_Position = vec4(position, 0.0, 1.0);
            }
        )glsl";
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        GLint status;
        char buffer[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
        glGetShaderInfoLog(vertexShader, 512, NULL, buffer);

        const char* fragmentSource = R"glsl(
            #version 150 core

            out vec4 outColor;

            void main()
            {
                outColor = vec4(1.0, 1.0, 1.0, 1.0);
            }
        )glsl";
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
        
        glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glBindFragDataLocation(shaderProgram, 0, "outColor");
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(posAttrib);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        window->SwapBuffers();
    }

    static void ErrorCheck()
    {
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