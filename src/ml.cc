
#include <iostream>

#include "render.h"
#include "ann.h"


int main(int argc, char *argv[])
{
    double angle1 = std::atan2(0, 100);
    double angle2 = std::atan2(100, 0);
    double angle3 = std::atan2(0, -100);
    double angle4 = std::atan2(-100, 0);
    std::cout << angle1 << " " << angle2 << " " << angle3 << " " << angle4 << std::endl;
    org::Network net({{2, nullptr}, {3, std::make_shared<org::act::Tanh>()}, {1, std::make_shared<org::act::Tanh>()}});
    //org::Render::Instance()->Init("Demo", 1000, 1000);

    org::Render::Instance()->Run();

    return 0;
}