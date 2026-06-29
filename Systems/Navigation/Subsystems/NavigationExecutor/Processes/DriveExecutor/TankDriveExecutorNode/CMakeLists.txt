add_executable(test_node src/test_node.cpp)
target_link_libraries(test_node ${catkin_LIBRARIES} standard_msgs base_DriveExecutorProcess)