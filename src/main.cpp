#include <iostream>
#include "graphics.h"


int main(int argc, char *argv[])
{
    GraphicsEngine Graphics = GraphicsEngine();
    WindowManager window = WindowManager();
    Graphics.AttachWindow(&window);
    //Graphics.Render();
    return 0;
}