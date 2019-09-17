# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (YareEngine)

add_subdirectory(Assets)

# ------------------- All source files ----------------------------------------------
file(GLOB_RECURSE YARE_HEADERS CONFIGURE_DEPENDS "Yare/*.hpp")
file(GLOB_RECURSE YARE_SOURCE  CONFIGURE_DEPENDS "Yare/*.cpp")

# -------------------- Structure -----------------------------------------------------
source_group(Yare REGULAR_EXPRESSION "Yare/.*")
    source_group(Yare\\OS REGULAR_EXPRESSION "Yare/OS/.*")
    source_group(Yare\\Memory REGULAR_EXPRESSION "Yare/Memory/.*")
    source_group(Yare\\Graphics REGULAR_EXPRESSION "Yare/Graphics/.*")
        source_group(Yare\\Graphics\\OpenGL REGULAR_EXPRESSION "Yare/Graphics/OpenGL/.*")
        source_group(Yare\\Graphics\\Materials REGULAR_EXPRESSION "Yare/Graphics/Materials/.*")
    source_group(Yare\\Geometry REGULAR_EXPRESSION "Yare/Geometry/.*")
    
       
# --------------------- Add source           -----------------------------------------
add_library(${PROJECT_NAME} ${YARE_SOURCE} ${YARE_HEADERS} )

# --------------------- Set Compile Time Flags ---------------------------------------
target_compile_definitions(${PROJECT_NAME} PUBLIC YARE_PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(${PROJECT_NAME} PUBLIC YARE_DEBUG=1)

# ------------------- Includes   -----------------------------------------------------
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "ThirdParty/stb")

# -------------------     Defs   -----------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_property(TARGET YareEngine PROPERTY CXX_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall)

# -----------------------       Third Party Libs    ----------------------------------

# glfw
add_subdirectory(ThirdParty/glfw EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(ThirdParty/glew EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PUBLIC libglew_static)

# glm (header only, but linked to Engine project to prevent sandbox from having to include)
add_subdirectory(ThirdParty/glm EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)


