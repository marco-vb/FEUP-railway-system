cmake_minimum_required(VERSION 3.23)
project(railways)

set(CMAKE_CXX_STANDARD 20)

add_executable(railways main.cpp classes/Station.cpp classes/Station.h classes/Line.cpp classes/Line.h classes/Link.cpp classes/Link.h)
