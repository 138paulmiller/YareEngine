cmake_minimum_required (VERSION 3.1)
project (YareEngineContent)

file(GLOB_RECURSE CONTENT CONFIGURE_DEPENDS "*/*.*")

source_group(Models REGULAR_EXPRESSION "Models/.*")
source_group(Shaders REGULAR_EXPRESSION "Shaders/.*")
source_group(Textures REGULAR_EXPRESSION "Textures/.*")

add_custom_target(${PROJECT_NAME}
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
				   SOURCES ${CONTENT}
)