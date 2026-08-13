add_library(commonWindowUtility CommonWindowUtility.cpp)
target_link_libraries(commonWindowUtility ${catkin_LIBRARIES}
                      ${CURSES_LIBRARIES})
add_dependencies(commonWindowUtility ${robot_framework_ros_EXPORTED_TARGETS})
target_include_directories(commonWindowUtility
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_library(baseWindow BaseWindow.cpp)
target_link_libraries(baseWindow ${catkin_LIBRARIES} ${CURSES_LIBRARIES}
                      commonWindowUtility)
add_dependencies(baseWindow ${robot_framework_ros_EXPORTED_TARGETS})
target_include_directories(baseWindow
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_library(headerWindow HeaderWindow.cpp)
target_link_libraries(headerWindow ${catkin_LIBRARIES} ${CURSES_LIBRARIES}
                      baseWindow)
add_dependencies(headerWindow ${robot_framework_ros_EXPORTED_TARGETS})
target_include_directories(headerWindow
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
