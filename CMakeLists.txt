cmake_minimum_required(VERSION 3.5)
project(Ex_3)
find_package(OpenCV REQUIRED )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES
    main.cpp
    sp_image_proc_util.h
    SPBPriorityQueue.c
    SPBPriorityQueue.h
    SPPoint.c
    SPPoint.h sp_image_proc_util.cpp)


add_executable(Ex_3 ${SOURCE_FILES})

target_link_libraries( Ex_3 ${OpenCV_LIBS} )

set_property(TARGET Ex_3 PROPERTY C_STANDARD 99)