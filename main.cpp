#include <iostream>

// our custom headers
#include "DiffFinder.hpp"

void printUsage()
{
    std::cout << "USAGE :\n ./DiffFinder path_to_another_image\n"; 
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printUsage();
        exit(-1);
    }

    DiffFinder *finderInstance = DiffFinder::getInstance();

    finderInstance->sayHi();
    finderInstance->setReference("/home/coker/Projects/DiffFinder/Resources/reference.png");
    Result res = finderInstance->isDifferent(argv[1], 0.048);

    if (Result::Different == res) {
        std::cout << "Images are Different\n";
    } else {
        std::cout << "Images are Almost Same\n";
    }

    DiffFinder::deleteInstance();

    return 0;
}