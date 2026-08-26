find_package(Qt5 REQUIRED COMPONENTS Widgets Core Charts UiTools)

# Include the headers relative to this subfolder
include_directories(include ${catkin_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
# This builds the library target that the root-level catkin_package exports
add_library(
  robot_framework_ros_gui include/robot_framework_ros/pid_tuner_plugin.h
                          include/robot_framework_ros/SmartDial.h
                          src/SmartDial.cpp
                          src/pid_tuner_plugin.cpp)

target_link_libraries(
  robot_framework_ros_gui
  ${catkin_LIBRARIES}
  Qt5::Widgets
  Qt5::Core
  Qt5::Charts
  Qt5::UiTools
  pid_Controller_shared)
