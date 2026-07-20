add_executable(nodeArmedStateManager ArmedStateManagerNode.cpp)
target_link_libraries(nodeArmedStateManager ${catkin_LIBRARIES}
                      ${Boost_LIBRARIES} baseNode armedStateManagerProcess)
add_dependencies(nodeArmedStateManager ${robot_framework_ros_EXPORTED_TARGETS})
if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
