cmake_minimum_required(VERSION 3.20)
project(simulation LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(zls src/main.c)

target_link_libraries(zls PRIVATE SDL3::SDL3)

target_compile_definitions(zls PRIVATE
    $<$<CONFIG:Debug>:FOOL_PROFING>
)