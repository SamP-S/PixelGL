#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb-master/stb_image.h"

namespace ImageInterface {

    unsigned char* LoadImage(const char* filepath, int* width, int* height, int* channels) {
        return stbi_load(filepath, width, height, channels, 0);
    }

    float* LoadImagef(const char* filepath, int* width, int* height, int* channels) {
        return stbi_loadf(filepath, width, height, channels, 0);
    }

    void FreeImage(unsigned char* data) {
        stbi_image_free(data);
    }

};