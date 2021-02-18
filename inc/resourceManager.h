#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1

#include <GL/gl.h>

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

}