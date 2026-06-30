add_library(baseNode BaseNode.cpp)
target_link_libraries(baseNode ${catkin_LIBRARIES})
#add_dependencies(baseNode robot_framework_ros_generate_messages_cpp)

if(CATKIN_ENABLE_TESTING)
    add_subdirectory(test)
endif()



install(TARGETS baseNode
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})