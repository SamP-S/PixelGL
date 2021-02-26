// Allows for external bindings of OpenGL functions
// SDL2 handles the rest
#define GL_GLEXT_PROTOTYPES 1

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef _WIN32 
    #include <gl/GL.h>
#else
    #include <GL/gl.h>
#endif

#include <assert.h>
#include "m3d.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FLAGS SDL_WINDOW_OPENGL

class ResoureManager {
    private:
    GLuint vbos[64];
    GLuint vaos[8];
    GLuint textures[64];

    ResoureManager() {

    }

    bool LoadModel() {
        return true;
    }

    bool LoadTexure() {
        return true;
    }
};

class WindowManager {
    public:
    //static int winWidth;
    //static int winHeight;

    SDL_GLContext context;
    SDL_Window* window_ptr;

    WindowManager(void) {

        // Initialise SDL
        SDL_Init(SDL_INIT_VIDEO);

        // create window
        this->window_ptr = SDL_CreateWindow("OpenGL 3.0", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
        assert(window_ptr);

        // create context
        context = SDL_GL_CreateContext(window_ptr);

        // get event
        SDL_Event Event;
        SDL_PollEvent(&Event);
        //SDL_Renderer* displayRenderer;
        //SDL_RendererInfo displayRendererInfo;
        //SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
        //SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
    }

    ~WindowManager(void) {
        SDL_Quit();
    }

    void PollEvents() {
        SDL_Event event;
        // SDL_PollEvent returns 1 while there is an event in the queue
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                // Window event
                case SDL_WINDOWEVENT:
                    break;
                // Keyboard event
                case SDL_KEYDOWN:
                    break;
                case SDL_KEYUP:
                    break;
                // Mouse button event
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEBUTTONUP:
                    break;
                case SDL_MOUSEWHEEL:
                    break;
                // Mouse movement event
                case SDL_MOUSEMOTION:
                    break;
                // Exit event
                case SDL_QUIT:
                    break;
            }
        }

    }

    void SwapBuffers() {
        SDL_GL_SwapWindow(window_ptr);
        SDL_Delay(2000);
    }

};



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
    }

    void InitGL()
    {
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

        /* change to the projection matrix and set our viewing volume. */
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );

        /* Make sure we're chaning the model view and not the projection */
        glMatrixMode( GL_MODELVIEW );

        /* Reset The View */
        glLoadIdentity( );

        return 1;
    }

    void Render()
    {
        /* Set the background black */
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        /* Clear The Screen And The Depth Buffer */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glLoadIdentity();
        glTranslatef( -1.0f, 0.0f, 0.0f);

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin( GL_TRIANGLES );            /* Drawing Using Triangles */
        glVertex3f(  0.0f,  1.0f, 0.0f ); /* Top */
        glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
        glVertex3f(  1.0f, -1.0f, 0.0f ); /* Bottom Right */
        glEnd( );                           /* Finished Drawing The Triangle */

        glTranslatef( 2.0f, 1.0f, 0.0f );

        glBegin( GL_QUADS );                /* Draw A Quad */
        glVertex3f( -1.0f,  1.0f, 0.0f ); /* Top Left */
        glVertex3f(  1.0f,  1.0f, 0.0f ); /* Top Right */
        glVertex3f(  1.0f, -1.0f, 0.0f ); /* Bottom Right */
        glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
        glEnd( );                           /* Done Drawing The Quad */

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