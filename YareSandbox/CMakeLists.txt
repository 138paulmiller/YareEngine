cmake_minimum_required(VERSION 3.0)
project(YareSandbox)

add_subdirectory(Assets)

file(GLOB_RECURSE YARESANDBOX_HEADERS CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE YARESANDBOX_SOURCE  CONFIGURE_DEPENDS "*.cpp")

add_executable(${PROJECT_NAME} ${YARESANDBOX_SOURCE} ${YARESANDBOX_HEADERS})


target_compile_definitions(${PROJECT_NAME} PUBLIC YARE_PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Include Yare 
target_include_directories(${PROJECT_NAME} PRIVATE "../YareEngine")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY Build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY Build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY Build)
set(CMAKE_BINARY_OUTPUT_DIRECTORY Build)

target_link_libraries(YareSandbox PUBLIC YareEngine)

foreach( i ${YARESANDBOX_ASSETS})
message(${i}   )
add_custom_command(TARGET ${PROJECT_NAME} 
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}
          COMMAND ${CMAKE_COMMAND} -E copy ${YARESANDBOX_ASSETS} ${PROJECT_BINARY_DIR}
)
endforeach(i)
