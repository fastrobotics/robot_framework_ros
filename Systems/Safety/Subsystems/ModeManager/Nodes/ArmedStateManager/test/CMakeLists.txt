add_rostest_gtest(test_nodeArmedStateManager test_ArmedStateManagerNode.test
                  test_ArmedStateManagerNode.cpp)
add_dependencies(test_nodeArmedStateManager nodeArmedStateManager)
target_link_libraries(
  test_nodeArmedStateManager ${catkin_LIBRARIES} ${Boost_LIBRARIES}
  ${GTEST_LIBRARIES} armedStateManagerProcess translateUtility)
