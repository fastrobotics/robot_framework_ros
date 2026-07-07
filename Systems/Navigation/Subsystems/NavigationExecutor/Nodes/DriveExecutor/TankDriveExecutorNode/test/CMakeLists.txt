add_rostest_gtest(test_nodeTankDriveExecutor test_TankDriveExecutorNode.test
                  test_TankDriveExecutorNode.cpp)
add_dependencies(test_nodeTankDriveExecutor nodeTankDriveExecutor)
target_link_libraries(
  test_nodeTankDriveExecutor ${catkin_LIBRARIES} ${Boost_LIBRARIES}
  ${GTEST_LIBRARIES} TankDriveExecutorProcess)
