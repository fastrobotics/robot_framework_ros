add_library(translateUtility TranslateUtility.cpp)
target_link_libraries(translateUtility ${catkin_LIBRARIES} infrastructure_msgs
                      geometry_msgs sensor_msgs)
add_dependencies(translateUtility ${robot_framework_ros_EXPORTED_TARGETS})

if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
