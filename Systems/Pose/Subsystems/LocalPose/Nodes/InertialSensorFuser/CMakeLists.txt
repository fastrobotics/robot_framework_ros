add_executable(nodeInertialSensorFuser InertialSensorFuserNode.cpp)
target_link_libraries(
  nodeInertialSensorFuser ${catkin_LIBRARIES} ${Boost_LIBRARIES} baseNode
  base_InertialSensorFuserProcess basic_InertialSensorFuserProcess)
add_dependencies(nodeInertialSensorFuser
                 ${robot_framework_ros_EXPORTED_TARGETS})
if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
