#include <iostream>
#include "DiffFinder.hpp"

int main(int argc, char** argv)
{
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