include_directories(include ${catkin_INCLUDE_DIRS})
add_library(
  robot_framework_ros_gui include/robot_framework_ros/pid_tuner_plugin.h
                          src/pid_tuner_plugin.cpp)

# 1. Link libraries
target_link_libraries(robot_framework_ros_gui ${catkin_LIBRARIES} Qt5::Widgets)
catkin_package(
  INCLUDE_DIRS
  include
  LIBRARIES
  robot_framework_ros_gui
  CATKIN_DEPENDS
  roscpp
  rqt_gui
  rqt_gui_cpp
  std_msgs)
