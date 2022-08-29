
#include <iostream>

#include "render.h"
#include "ann.h"


int main(int argc, char *argv[])
{
    org::Network net;
    net.Construct({{2, nullptr}, {3, std::make_shared<org::Tanh>()}, {1, std::make_shared<org::Tanh>()}});
    org::Render::Instance()->Init("Demo", 800, 600);

    org::Render::Instance()->Run();

    return 0;
}