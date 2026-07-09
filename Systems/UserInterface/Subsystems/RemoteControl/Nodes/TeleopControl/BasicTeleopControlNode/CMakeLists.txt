add_executable(nodeBasicTeleopControl BasicTeleopControlNode.cpp)
target_link_libraries(nodeBasicTeleopControl ${catkin_LIBRARIES}
                      ${Boost_LIBRARIES} baseNode basic_TeleopControlProcess)
add_dependencies(nodeBasicTeleopControl ${robot_framework_ros_EXPORTED_TARGETS})
if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
