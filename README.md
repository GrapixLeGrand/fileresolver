# NanoFileResolver

Small and minimal API to lookup path. Managing path when opening and writing to files can be annoying in c++. This small library provides a way to define a new origin directory and to make subsequent path query relatively to this origin directory.

# Building

The library is header only. Simply copy past the fileresolver.hpp in the include folder.

# Usage

Given the following `tree` command output:

```
├── CMakeLists.txt
├── include
│   └── fileresolver.hpp
├── LICENSE
├── main.cpp
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

The following piece of code would create the file resolver.

```
Grapix::FileResolver resolver = Grapix::FileResolverFactory::makeRecursiveResolver("folder2");
resolver.getAbsolutePath("sub2/sub2.txt") // == <path-root-to-folder2>/sub2/sub2.txt
``` 

There exists two resolver types. The `LinearResolver` and the `RecursiveResolver`. The linear one is a toy example. It will simply look back along the path if the target folder is present. For example in `/my/path/is/good` it will start from `good`, then `is` etc. until it finds the target directory. This is a toy example because it is not very adaptive to complex project structure. The recursive file resolver is going to perform a breath-first-search from the compiled binary. It will perform the search until `maxDepth` is reached and will throw an exception if the path isn't found.

# Todo

- check if the absolute path is existing
- add the possiblity to ls

# Licence

MIT License, see licence file