# SimpleMemory

This is a simple memory with DMI using **GreenLib**.

## Code status

[![build status](http://ci.greensocs.com/projects/9/status.png?ref=master)](http://ci.greensocs.com/projects/9?ref=master)

## Include this model in a project

* Copy the `misc/FindSIMPLEMEMORY.cmake` in your project repository.
* Add the following lines into your `CMakeLists.txt`:

``` cmake
find_package(SIMPLEMEMORY)
if(SIMPLEMEMORY_FOUND)
    include_directories(${SIMPLEMEMORY_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "SimpleMemory not found.")
endif()
```

You're now able to use **SimpleMemory** headers.

## Contributing

**SimpleMemory** is an open source, community-driven project. If you'd like to contribute, please feel free to fork project and open a merge request or to [send us] a patch. 

[send us]:http://www.greensocs.com/contact

![GreenLib-logo](http://static.greensocs.com/logo.png)
