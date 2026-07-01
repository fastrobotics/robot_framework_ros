catkin_add_gtest(test_coreUtility test_CoreUtility.cpp)
target_link_libraries(test_coreUtility ${catkin_LIBRARIES} ${Boost_LIBRARIES} coreUtility)
add_dependencies(test_coreUtility ${robot_framework_ros_EXPORTED_TARGETS})