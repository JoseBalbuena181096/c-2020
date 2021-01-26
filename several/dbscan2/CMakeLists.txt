cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0012 NEW)
PROJECT(projet)

add_library(
    dbscan
    dbscan.h
    dbscan.cpp
)

ADD_EXECUTABLE(db main.cpp)
target_link_libraries(db PRIVATE dbscan)