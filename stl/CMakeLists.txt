cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0012 NEW)
PROJECT(projet)

ADD_EXECUTABLE(template template.cpp)



ADD_EXECUTABLE(vector vector.cpp)

find_package(Eigen3 REQUIRED)

link_directories(${Eigen_INCLUDE_DIRS})

add_executable(Eigen Eigen.cpp)
target_link_libraries(Eigen ${Eigen_LIBRARIES})

