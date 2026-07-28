add_executable(nodeIMU IMUNode.cpp)
target_link_libraries(nodeIMU ${catkin_LIBRARIES} ${Boost_LIBRARIES} baseNode
                      IMUProcess)
add_dependencies(nodeIMU ${robot_framework_ros_EXPORTED_TARGETS})
if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
