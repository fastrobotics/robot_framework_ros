add_executable(nodeTankDriveExecutor TankDriveExecutorNode.cpp)
target_link_libraries(nodeTankDriveExecutor ${catkin_LIBRARIES}
                      ${Boost_LIBRARIES} baseNode TankDriveExecutorProcess)
add_dependencies(nodeTankDriveExecutor ${robot_framework_ros_EXPORTED_TARGETS})
if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
