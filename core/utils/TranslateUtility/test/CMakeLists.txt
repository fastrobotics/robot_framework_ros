catkin_add_gtest(test_translateUtility test_TranslateUtility.cpp)
target_link_libraries(test_translateUtility ${catkin_LIBRARIES}
                      ${Boost_LIBRARIES} translateUtility)
add_dependencies(test_translateUtility ${robot_framework_ros_EXPORTED_TARGETS})
