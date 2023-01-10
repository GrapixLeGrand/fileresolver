#include <iostream>
#include <chrono>

#include "include/fileresolver.hpp"

#define STARTTIMING { std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

#define ENDTIMING(FOLDER) \
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); \
std::cout << "Elapse to find the folder " << FOLDER << " = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl; }


int main(int, char**) {

    //  ├── CMakeLists.txt
    //  ├── include
    //  │   └── fileresolver.hpp
    //  ├── LICENSE
    //  ├── demo.cpp
    //  ├── README.md
    //  ├── tests
    //  │   ├── folder1
    //  │   │   ├── file1
    //  │   │   └── file2
    //  │   └── folder2
    //  │       ├── file3
    //  │       ├── sub1
    //  │       │   └── sub1.txt
    //  │       ├── sub2
    //  │       │   └── sub2.txt
    //  │       └── sub3
    //  │           ├── sub3.txt
    //  │           └── sub4
    //  │               └── myfile
    //  └── tests.cpp


    STARTTIMING
        printf("recursive file resolver:\n");
        Grapix::FileResolver* resolver = Grapix::FileResolverFactory::makeRecursiveResolverPtr("folder1");
        printf("--> origin path %s\n", resolver->getOriginPath().c_str());
        printf("--> absolute to file1 %s\n", resolver->getAbsolute("file1").c_str());
        printf("tostring: %s\n", resolver->toString().c_str());
    ENDTIMING("folder1")

    STARTTIMING
        printf("recursive file resolver:\n");
        Grapix::FileResolver* resolver = Grapix::FileResolverFactory::makeRecursiveResolverPtr("sub2");
        printf("--> origin path %s\n", resolver->getOriginPath().c_str());
        printf("--> absolute to file1 %s\n", resolver->getAbsolute("sub2.txt").c_str());
        printf("tostring: %s\n", resolver->toString().c_str());
    ENDTIMING("sub2")

    STARTTIMING
        printf("recursive file resolver:\n");
        Grapix::FileResolver* resolver = Grapix::FileResolverFactory::makeRecursiveResolverPtr("sub4");
        printf("--> origin path %s\n", resolver->getOriginPath().c_str());
        printf("--> absolute to file1 %s\n", resolver->getAbsolute("myfile").c_str());
        printf("tostring: %s\n", resolver->toString().c_str());
    ENDTIMING("sub4")

    
}
