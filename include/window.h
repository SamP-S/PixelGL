#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <assert.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FLAGS SDL_WINDOW_OPENGL

class WindowManager {
    public:
        //static int winWidth;
        //static int winHeight;

        SDL_GLContext context;
        SDL_Window* window_ptr;
        int width = WINDOW_WIDTH;
        int height = WINDOW_HEIGHT;

        WindowManager() : WindowManager(WINDOW_WIDTH, WINDOW_HEIGHT) {
        }

        WindowManager(int _width, int _height) {
            this->width = _width;
            this->height = _height;

            // Initialise SDL
            SDL_Init(SDL_INIT_VIDEO);
            SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
            SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
            SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

            // create window
            this->window_ptr = SDL_CreateWindow("OpenGL 3.0", 0, 0, width, height, WINDOW_FLAGS);
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

        ~WindowManager() {
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
        // SDL_Delay(2000);
    }

    void Delay(int ms) {
        SDL_Delay(ms);
    }

};