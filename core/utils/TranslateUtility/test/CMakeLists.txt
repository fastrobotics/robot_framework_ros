catkin_add_gtest(test_translateUtilityStandardMsgs
                 test_TranslateUtilityStandardMsgs.cpp)
target_link_libraries(test_translateUtilityStandardMsgs ${catkin_LIBRARIES}
                      ${Boost_LIBRARIES} translateUtility)
add_dependencies(test_translateUtilityStandardMsgs
                 ${robot_framework_ros_EXPORTED_TARGETS})

catkin_add_gtest(test_translateUtilityInfrastructureMsgs
                 test_TranslateUtilityInfrastructureMsgs.cpp)
target_link_libraries(test_translateUtilityInfrastructureMsgs
                      ${catkin_LIBRARIES} ${Boost_LIBRARIES} translateUtility)
add_dependencies(test_translateUtilityInfrastructureMsgs
                 ${robot_framework_ros_EXPORTED_TARGETS})

catkin_add_gtest(test_translateUtilityGeometryMsgs
                 test_TranslateUtilityGeometryMsgs.cpp)
target_link_libraries(test_translateUtilityGeometryMsgs ${catkin_LIBRARIES}
                      ${Boost_LIBRARIES} translateUtility)
add_dependencies(test_translateUtilityGeometryMsgs
                 ${robot_framework_ros_EXPORTED_TARGETS})

catkin_add_gtest(test_translateUtilitySensorMsgs
                 test_TranslateUtilitySensorMsgs.cpp)
target_link_libraries(test_translateUtilitySensorMsgs ${catkin_LIBRARIES}
                      ${Boost_LIBRARIES} translateUtility)
add_dependencies(test_translateUtilitySensorMsgs
                 ${robot_framework_ros_EXPORTED_TARGETS})
