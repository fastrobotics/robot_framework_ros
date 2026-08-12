add_rostest_gtest(
  test_nodeTrajectoryController test_TrajectoryControllerNode.test
  test_TrajectoryControllerNode.cpp)
add_dependencies(test_nodeTrajectoryController nodeTrajectoryController)
target_link_libraries(
  test_nodeTrajectoryController ${catkin_LIBRARIES} ${Boost_LIBRARIES}
  ${GTEST_LIBRARIES} basic_TrajectoryControllerProcess)
