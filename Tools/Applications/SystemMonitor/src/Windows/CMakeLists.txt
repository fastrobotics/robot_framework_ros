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

add_library(nodeInfoWindow NodeInfoWindow.cpp)
target_link_libraries(nodeInfoWindow ${catkin_LIBRARIES} ${CURSES_LIBRARIES}
                      baseWindow)
add_dependencies(nodeInfoWindow ${robot_framework_ros_EXPORTED_TARGETS})
target_include_directories(nodeInfoWindow
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_library(diagnosticWindow DiagnosticWindow.cpp)
target_link_libraries(diagnosticWindow ${catkin_LIBRARIES} ${CURSES_LIBRARIES}
                      baseWindow)
add_dependencies(diagnosticWindow ${robot_framework_ros_EXPORTED_TARGETS})
target_include_directories(diagnosticWindow
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
