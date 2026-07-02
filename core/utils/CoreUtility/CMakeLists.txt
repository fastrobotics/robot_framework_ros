add_library(coreUtility CoreUtility.cpp)
target_link_libraries(coreUtility ${catkin_LIBRARIES})
add_dependencies(coreUtility ${robot_framework_ros_EXPORTED_TARGETS})

if(CATKIN_ENABLE_TESTING)
    add_subdirectory(test)
endif()
