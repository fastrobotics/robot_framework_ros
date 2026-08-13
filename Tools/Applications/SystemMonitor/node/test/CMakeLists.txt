add_rostest_gtest(test_nodeSystemMonitor test_SystemMonitorNode.test
                  test_SystemMonitorNode.cpp)
add_dependencies(test_nodeSystemMonitor nodeSystemMonitor)
target_link_libraries(
  test_nodeSystemMonitor ${catkin_LIBRARIES} ${Boost_LIBRARIES}
  ${GTEST_LIBRARIES} SystemMonitorProcess)
