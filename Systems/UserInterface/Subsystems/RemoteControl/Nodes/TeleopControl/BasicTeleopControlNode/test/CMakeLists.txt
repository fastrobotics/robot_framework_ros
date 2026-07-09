add_rostest_gtest(test_BasicTeleopControlNode test_BasicTeleopControlNode.test
                  test_BasicTeleopControlNode.cpp)
add_dependencies(test_BasicTeleopControlNode nodeBasicTeleopControl)
target_link_libraries(
  test_BasicTeleopControlNode ${catkin_LIBRARIES} ${Boost_LIBRARIES}
  ${GTEST_LIBRARIES} basic_TeleopControlProcess)
