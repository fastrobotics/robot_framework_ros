add_rostest_gtest(test_nodeIMU test_IMUNode.test test_IMUNode.cpp)
add_dependencies(test_nodeIMU nodeIMU)
target_link_libraries(test_nodeIMU ${catkin_LIBRARIES} ${Boost_LIBRARIES}
                      ${GTEST_LIBRARIES} IMUProcess)
