cmake_minimum_required(VERSION 3.5)
project(Ex_3)
find_package(OpenCV REQUIRED )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message(STATUS "OpenCV_LIBS=${OpenCV_LIBS}")

set(SOURCE_FILES
    main.cpp
    main_aux.h
    main_aux.cpp
    sp_image_proc_util.h
    sp_image_proc_util.cpp
    SPBPriorityQueue.c
    SPBPriorityQueue.h
    SPPoint.c
    SPPoint.h)


add_executable(Ex_3 ${SOURCE_FILES})

target_link_libraries( Ex_3 ${OpenCV_LIBS})

set_property(TARGET Ex_3 PROPERTY C_STANDARD 99)