cmake_minimum_required(VERSION 3.28)
project(Dualie_Demo)
set(CMAKE_CXX_STANDARD 17)
set(Dualie_DIR "" CACHE PATH "Path to Dualie CMake config directory")
if(Dualie_DIR)
        list(PREPEND CMAKE_PREFIX_PATH "${Dualie_DIR}")
endif()
find_package(Dualie REQUIRED)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${DUALIE_LIBRARY})
target_compile_options(${PROJECT_NAME} PRIVATE
        -O2 -Wall -Wextra
)
ctr_generate_smdh(${PROJECT_NAME}.smdh
        NAME "${PROJECT_NAME}"
        DESCRIPTION "v1.0.0"
        AUTHOR "SOMEBODY"
)
ctr_create_3dsx(${PROJECT_NAME}
        SMDH ${PROJECT_NAME}.smdh
)