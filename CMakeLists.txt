cmake_minimum_required(VERSION 3.0.2)
project(robot_framework_ros)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage -fpermissive ${WARNING_FLAGS}"
)
if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL armv7l)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DARCHITECTURE_ARMV7L")
endif()
add_definitions(-std=c++11 -Wall -Wextra -Wno-switch -Werror)

# Find catkin macros and libraries if COMPONENTS list like find_package(catkin
# REQUIRED COMPONENTS xyz) is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs sensor_msgs
                                        message_generation genmsg)
find_package(GTest REQUIRED)
find_package(rostest REQUIRED)

add_message_files(DIRECTORY msg FILES heartbeat.msg nodestate.msg)
generate_messages(DEPENDENCIES std_msgs)

catkin_package(INCLUDE_DIRS include DEPENDS)

include_directories(${catkin_INCLUDE_DIRS} include)
include(FetchContent)

# Declare GoogleTest dependency
FetchContent_Declare(
  robot_framework
  GIT_REPOSITORY https://github.com/fastrobotics/robot_framework.git
  GIT_SHALLOW ON
  GIT_TAG master)
set(BUILD_TESTING
    OFF
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(robot_framework)
add_subdirectory(core)
# add_subdirectory(Systems)
