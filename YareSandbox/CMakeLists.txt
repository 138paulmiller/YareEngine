cmake_minimum_required(VERSION 3.0)
project(YareSandbox)

add_subdirectory(Assets)

file(GLOB_RECURSE YARESANDBOX_HEADERS CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE YARESANDBOX_SOURCE  CONFIGURE_DEPENDS "*.cpp")

add_executable(${PROJECT_NAME} ${YARESANDBOX_SOURCE} ${YARESANDBOX_HEADERS})

target_compile_definitions(${PROJECT_NAME} PUBLIC YARESANDBOX_PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")


# Include and link Yare Engine 
target_include_directories(${PROJECT_NAME} PRIVATE "../YareEngine")
target_link_libraries(${PROJECT_NAME} PUBLIC YareEngine)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY Build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY Build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY Build)
set(CMAKE_BINARY_OUTPUT_DIRECTORY Build)


