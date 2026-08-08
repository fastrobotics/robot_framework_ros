add_rostest_gtest(
  test_nodeInertialSensorFuser test_InertialSensorFuserNode.test
  test_InertialSensorFuserNode.cpp)
add_dependencies(test_nodeInertialSensorFuser nodeInertialSensorFuser)
target_link_libraries(
  test_nodeInertialSensorFuser ${catkin_LIBRARIES} ${Boost_LIBRARIES}
  ${GTEST_LIBRARIES} base_InertialSensorFuserProcess)
