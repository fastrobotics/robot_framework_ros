add_executable(nodeServoHat ServoHatNode.cpp)
target_link_libraries(nodeServoHat ${catkin_LIBRARIES} ${Boost_LIBRARIES}
                      baseNode ServoHatProcess)
add_dependencies(nodeServoHat ${robot_framework_ros_EXPORTED_TARGETS})
if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
