add_rostest_gtest(test_nodeLocalPoseFuser test_LocalPoseFuserNode.test
                  test_LocalPoseFuserNode.cpp)
add_dependencies(test_nodeLocalPoseFuser nodeLocalPoseFuser)
target_link_libraries(
  test_nodeLocalPoseFuser ${catkin_LIBRARIES} ${Boost_LIBRARIES}
  ${GTEST_LIBRARIES} base_LocalPoseFuserProcess)
