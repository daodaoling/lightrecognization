cmake_minimum_required(VERSION 3.0.2)
project(lightrecognization)

add_definitions(-std=c++11)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  rosbag
  cv_bridge
  tf
  #OpenCV
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES example_cpp
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

set(CUDA_NVCC_PLAGS ${CUDA_NVCC_PLAGS};-std=c++11;-g;-G;-gencode;arch=compute_52;code=sm_52)

include_directories(${PROJECT_SOURCE_DIR}/include)
if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    message("embed_platform on")
    include_directories(/usr/local/cuda/targets/aarch64-linux/include)
    link_directories(/usr/local/cuda/targets/aarch64-linux/lib)
else()
    message("embed_platform off")
    include_directories(/usr/local/cuda/include)
    link_directories(/usr/local/cuda/lib64)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")


include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if (COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(FATAL_ERROR "${CMAKE_CXX_COMPILER} doesn't provide c++11 support.")
endif()

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  #${OpenCV_INCLUDE_DIRS}
)

include_directories(
 include
  ${catkin_INCLUDE_DIRS}
 /usr/local/cuda/lib64
 src
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")

#cuda_add_library(leaky ${PROJECT_SOURCE_DIR}/leaky.cu)
cuda_add_library(yololayer SHARED ${PROJECT_SOURCE_DIR}/include/yololayer.cu)

find_package(OpenCV)
include_directories(OpenCV_INCLUDE_DIRS)

add_executable(lightrecognization src/lightrecognization.cpp)

add_dependencies(lightrecognization example_cpp_generate_messages_cpp)

target_link_libraries(lightrecognization  ${catkin_LIBRARIES} ${OpenCV_LIBS} )#ros_cam_test
target_link_libraries(lightrecognization nvinfer)
target_link_libraries(lightrecognization cudart)
target_link_libraries(lightrecognization yololayer)
target_link_libraries(lightrecognization ${OpenCV_LIBS})
#ADD_DEPENDENCIES(main Cudalib)
#add_definitions(-O2 -pthread)
