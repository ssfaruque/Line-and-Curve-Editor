#include <iostream>
#include <string>
#include "app.h"


int main(int argc, char** argv)
{
    if(argc == 3)
    {
        App::create("Assignment 1", std::stoi(argv[1]), std::stoi(argv[2]), &argc, argv);
        App* app = App::getInstance();
        app->run();
    }

    else
        std::cout << "Usage: ./out windowWidth windowHeight" << std::endl;

    return 0;
}