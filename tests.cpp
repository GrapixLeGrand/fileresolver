#include <iostream>

#include "include/fileresolver.hpp"


int testfun(const std::string& originDir, const std::string& fileRelative) {
    Grapix::FileResolver resolver = Grapix::FileResolverFactory::makeRecursiveResolver(originDir);
    std::string fullPath = resolver.getAbsolute(fileRelative);
    std::string intialPath = resolver.getCompiledPath();

    std::cout << fullPath << std::endl;
    std::cout << intialPath << std::endl;

    if (fullPath.find(intialPath) != 0) {
        return 1;
    }

    return 0;
}

#define RUN_CTEST(TESTNAME, fun) {if(!(fun)) {printf("test %s successful\n", (TESTNAME)); } else { printf("test %s failed\n", (TESTNAME)); result |= 1;} }

int main(int, char**) {
    
    //better to try to open some file relatively to the origin.
    
    int result = 0;
    //RUN_CTEST("folder1", testfun("folder1", "file1"));
    //RUN_CTEST("sub2", testfun("sub2", "sub2.txt"));

/*
    STARTTIMING
        printf("recursive file resolver:\n");
        Grapix::FileResolver* resolver = Grapix::FileResolverFactory::makeRecursiveResolverPtr("sub4");
        printf("--> origin path %s\n", resolver->getOriginPath().c_str());
        printf("--> absolute to file1 %s\n", resolver->getAbsolute("myfile").c_str());
        printf("tostring: %s\n", resolver->toString().c_str());
    ENDTIMING("sub4")
    */

    return result;
}