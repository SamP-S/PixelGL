#include <iostream>
#include "graphics.h"


int main(int argc, char *argv[])
{
    WindowManager window = WindowManager(1920, 1080);
    GraphicsEngine Graphics = GraphicsEngine(&window);
    for (int i = 0; i < 1200; i++) {
        Graphics.Render();
    }
    std::cout << "end of program" << std::endl;
    return 0;
}