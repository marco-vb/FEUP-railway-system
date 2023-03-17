cmake_minimum_required(VERSION 3.23)
project(railways)

set(CMAKE_CXX_STANDARD 20)

add_executable(railways main.cpp classes/StationLink.cpp classes/StationLink.h classes/Network.cpp classes/Network.h)
