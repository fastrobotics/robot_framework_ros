add_executable(test_node src/test_node.cpp)
target_include_directories(test_node PRIVATE
  ${robot_framework_SOURCE_DIR}/Systems/Navigation/Subsystems/NavigationExecutor/Processes/DriveExecutor/include
  ${robot_framework_SOURCE_DIR}/msgs/StandardMsgs/include
  ${robot_framework_SOURCE_DIR}/msgs/GeometryMsgs/include
)
target_link_libraries(test_node ${catkin_LIBRARIES})