add_executable(node{{cookiecutter.Node}} {{cookiecutter.Node}}Node.cpp)
target_link_libraries(node{{cookiecutter.Node}} ${catkin_LIBRARIES}
                      ${Boost_LIBRARIES} baseNode {{cookiecutter.Node}}Process)
add_dependencies(node{{cookiecutter.Node}}
                 ${robot_framework_ros_EXPORTED_TARGETS})
if(CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif()
