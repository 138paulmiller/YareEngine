cmake_minimum_required (VERSION 3.1)
project (YareEngineAssets)

file(GLOB_RECURSE ASSETS  CONFIGURE_DEPENDS "*/*.*")

source_group(Models REGULAR_EXPRESSION "Models/.*")
source_group(Shaders REGULAR_EXPRESSION "Shaders/.*")
source_group(Images REGULAR_EXPRESSION "Images/.*")

message(${ASSETS})
add_custom_target(${PROJECT_NAME}
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
				   SOURCES ${ASSETS}
)