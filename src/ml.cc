
#include <iostream>

#include "render.h"



int main(int argc, char *argv[])
{
    org::Render::Instance()->Init("Demo", 800, 600);

    org::Render::Instance()->Run();

    return 0;
}