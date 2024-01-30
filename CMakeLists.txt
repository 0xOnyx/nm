cmake_minimum_required(VERSION 3.16)
project(nm C)

set(CMAKE_C_STANDARD 11)


add_executable(nm
        main.c)

target_link_libraries(nm bsd)
