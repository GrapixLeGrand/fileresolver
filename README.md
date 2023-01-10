# NanoFileResolver

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

[![CMake](https://github.com/GrapixLeGrand/fileresolver/actions/workflows/cmake.yml/badge.svg)](https://github.com/GrapixLeGrand/fileresolver/actions/workflows/cmake.yml)

Small and minimal API to lookup path. Managing path when opening and writing to files can be annoying in c++. This small library provides a way to define a new origin directory and to make subsequent path query relatively to this origin directory. Usually, the path are relatives to where you call your binary. This may couple your source code with the location you must start your program from. Having an abstraction that defines a unique origin directory can help in some cases. This is what NanoFileResolver is trying to offer.


# Usage

You can observe the following in the demo.cpp file. Given the following `tree` command output:

```
├── CMakeLists.txt
├── include
│   └── fileresolver.hpp
├── LICENSE
├── demo.cpp
├── README.md
├── tests
│   ├── folder1
│   │   ├── file1
│   │   └── file2
│   └── folder2
│       ├── file3
│       ├── sub1
│       │   └── sub1.txt
│       ├── sub2
│       │   └── sub2.txt
│       └── sub3
│           ├── sub3.txt
│           └── sub4
│               └── myfile
└── tests.cpp
```

The following piece of code would create the file resolver with target folder `folder2`. 

```
Grapix::FileResolver resolver = Grapix::FileResolverFactory::makeRecursiveResolver("folder2");
resolver.getAbsolutePath("sub2/sub2.txt") // == <path-root-to-folder2>/sub2/sub2.txt
``` 

There exists two resolver types. The `LinearResolver` and the `RecursiveResolver`. The linear one is a toy example. It will simply look back along the path if the target folder is present. For example in `/my/path/is/good` it will start from `good`, then `is` etc. until it finds the target directory. This is a toy example because it is not very adaptive to complex project structure. The recursive file resolver is going to perform a breath-first-search from the compiled binary. It will perform the search until `maxDepth` is reached and will throw an exception if the path isn't found.

# Building

The library is header only. Simply copy past the fileresolver.hpp in the include folder.

# Possible drawbacks

If the file arborescence is too big, the runtime of the recursive file resolver might be affected (only upon object creation).

# Todo

- check if the absolute path is existing
- add the possiblity to ls
- catch exception if we don't have permission to access some file.

# Licence

source code under [MIT License](LICENSE).