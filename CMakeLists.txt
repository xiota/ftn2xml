cmake_minimum_required(VERSION 3.5)

project(ftn2xml VERSION 0.1.0)

find_package(podofo REQUIRED)

option(VERSION "Project version" ${CMAKE_PROJECT_VERSION})

add_subdirectory(src)
