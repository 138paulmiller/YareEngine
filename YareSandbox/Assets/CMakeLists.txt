cmake_minimum_required (VERSION 3.1)
project (YareAssets)

file(GLOB_RECURSE ASSETS  CONFIGURE_DEPENDS "*/*.*")
message(${ASSETS})
source_group(Models REGULAR_EXPRESSION "Models/.*")
source_group(Shaders REGULAR_EXPRESSION "Shaders/.*")

add_custom_target(${PROJECT_NAME}
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
				   SOURCES ${ASSETS}
)



