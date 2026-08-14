add_library(headerWindow HeaderWindow.cpp)
target_link_libraries(headerWindow ${catkin_LIBRARIES} ${CURSES_LIBRARIES}
                      baseWindow)
add_dependencies(headerWindow ${robot_framework_ros_EXPORTED_TARGETS})
target_include_directories(headerWindow
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_library(statusWindow StatusWindow.cpp)
target_link_libraries(statusWindow ${catkin_LIBRARIES} ${CURSES_LIBRARIES}
                      baseWindow)
add_dependencies(statusWindow ${robot_framework_ros_EXPORTED_TARGETS})
target_include_directories(statusWindow
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
