# NanoFileResolver

Small and minimal API to lookup path. Managing path when opening and writing to files can be annoying in c++. This small library provides a way to define a new origin directory and to make subsequent path query relatively to this origin directory.

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

The following piece of code would create the fileresolver.

```
Grapix::FileResolver resolver = Grapix::FileResolverFactory::makeRecursiveResolver("folder2");
resolver.getAbsolutePath("sub2/sub2.txt") // == <path-root-to-folder2>/sub2/sub2.txt
``` 

# 
