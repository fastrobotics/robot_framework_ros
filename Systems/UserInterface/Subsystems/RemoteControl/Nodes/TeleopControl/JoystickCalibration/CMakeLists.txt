add_executable(nodeJoystickCalibration JoystickCalibrationNode.cpp)
target_link_libraries(nodeJoystickCalibration ${catkin_LIBRARIES}
                      ${Boost_LIBRARIES} baseNode basic_TeleopControlProcess)
add_dependencies(nodeJoystickCalibration
                 ${robot_framework_ros_EXPORTED_TARGETS})
