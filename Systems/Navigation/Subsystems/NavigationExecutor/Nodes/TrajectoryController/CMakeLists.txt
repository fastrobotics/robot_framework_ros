add_executable(nodeTrajectoryController TrajectoryControllerNode.cpp)
target_link_libraries(
  nodeTrajectoryController ${catkin_LIBRARIES} ${Boost_LIBRARIES} baseNode
  basic_TrajectoryControllerProcess)
add_dependencies(nodeTrajectoryController
                 ${robot_framework_ros_EXPORTED_TARGETS})
if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
