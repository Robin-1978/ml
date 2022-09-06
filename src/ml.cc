
#include <iostream>

#include "render.h"
#include "ann.h"

#include "calc_add.h"


void Test1()
{
    org::Network net({{2, nullptr}, {3, std::make_shared<org::act::Tanh>()}, {1, std::make_shared<org::act::Tanh>()}});
    //org::Render::Instance()->Init("Demo", 1000, 1000);

    org::Render::Instance()->Run();
}

void Test2()
{
    org::Calc::World world;
    world.Train();
    for(;;)
    {
        double a, b;
        std::cin >> a >> b;
        std::cout << world(a, b) << std::endl;

    }

}

int main(int argc, char *argv[])
{

    Test2();
    return 0;
}