add_executable(example_node ExampleNode.cpp) 
target_link_libraries(example_node ${catkin_LIBRARIES} ${Boost_LIBRARIES} baseNode )
add_dependencies(example_node ${robot_framework_ros_EXPORTED_TARGETS})
#add_subdirectory(test)