add_rostest_gtest(test_nodeServoHat test_ServoHatNode.test
                  test_ServoHatNode.cpp)
add_dependencies(test_nodeServoHat nodeServoHat)
target_link_libraries(test_nodeServoHat ${catkin_LIBRARIES} ${Boost_LIBRARIES}
                      ${GTEST_LIBRARIES} servoHatDriverProcess)
