#include <iostream>
#include <chrono>

#include "include/fileresolver.hpp"

#define STARTTIMING { std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

#define ENDTIMING(FOLDER) \
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); \
std::cout << "Elapse to find the folder " << FOLDER << " = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl; }


int main(int, char**) {
    
    STARTTIMING
        //simple linear resolver. Will iterate back to the root directory to find the origin target directory
        //from which it operates
        
        /*printf("linear file resolver:\n");
        Grapix::FileResolver resolver = Grapix::FileResolverFactory::makeLinearResolver("filesystem");
        printf("--> origin path %s\n", resolver.getOriginPath().c_str());
        printf("--> absolute to file1 %s\n", resolver.getAbsolute("tests/folder1/file1").c_str());
        printf("tostring: %s\n", resolver.toString().c_str());*/
    ENDTIMING("filesystem")

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
