add_executable(system_monitor SystemMonitorNode.cpp)
target_link_libraries(
  system_monitor
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
  baseNode
  headerWindow
  statusWindow
  nodeInfoWindow)
add_dependencies(system_monitor ${robot_framework_ros_EXPORTED_TARGETS})
