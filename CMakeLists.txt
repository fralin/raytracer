#Declaration du projet
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0012 NEW)
project (ray)

#require OpenCV
find_package( OpenCV 3.0.0 REQUIRED)
#show message with opencv version detected
message("OpenCV version : ${openCV_VERSION}")
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

#create a variable called SRC
set (SRC raytracer.cpp)

#create our executable 
add_executable(${PROJECT_NAME} ${SRC})

#link our library
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS})
