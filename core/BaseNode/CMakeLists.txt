add_library(baseNode BaseNode.cpp)
target_link_libraries(
  baseNode
  ${catkin_LIBRARIES}
  logger
  coreUtility
  translateUtility
  infrastructure_msgs
  geometry_msgs)
add_dependencies(baseNode ${robot_framework_ros_EXPORTED_TARGETS})

if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()

install(
  TARGETS baseNode
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})
