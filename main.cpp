#include <iostream>
#include "include/filesystem.hpp"


int main(int, char**) {

    {
        //simple linear resolver. Will iterate back to the root directory to find the origin target directory
        //from which it operates
        
        printf("linear file resolver:\n");
        Grapix::FileResolver resolver = Grapix::FileResolverFactory::makeLinearResolver("filesystem");
        printf("--> origin path %s\n", resolver.getOriginPath().c_str());
        printf("--> absolute to file1 %s\n", resolver.getAbsolute("tests/folder1/file1").c_str());
    }

    
}
