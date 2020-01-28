# Yare

Yes, another render engine

## Why?

For fun. 
*Do I need another reason?* 

-----------------------------------------------------------
## Installation 

Currently only supports Windows, all other platforms are untested. 
All dependencies are located under [YareEngine/ThirdParty](YareEngine/ThirdParty).

1. Install [CMake](https://cmake.org/download/) 
2. `git clone https://github.com/138paulmiller/YareEngine.git`

## Development

- Windows

1. Generate Engine and Sandbox Projects `cmake . -BProject`
2. Open Project/Yare.sln
3. Set YareSandbox as the Starup Project, and run!

## Building a new Project
See YareSandbox project for an example. Using Cmake, create a CMakeLists.txt in you prjects root directory. 
To link against the YareEngine project, use the following commands
```
target_include_directories(${PROJECT_NAME} PRIVATE "path/to/YareEngine")
target_link_libraries(${PROJECT_NAME} PUBLIC YareEngine)
```
Where `path/to/YareEngine` is the relative path from your CMakeLIsts.txt file to the YareEngine directory. 

NOTE : this is located under YareEngine/YareEngine if you clone the repo.

-----------------------------------------------------------

## Examples
Dynamic Shadows with 8 boxes and 8 directional lights
![](./Docs/Images/demo_8_boxes_8_dir_lights.png)
