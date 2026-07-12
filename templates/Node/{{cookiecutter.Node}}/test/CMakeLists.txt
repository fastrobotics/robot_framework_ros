add_rostest_gtest(
  test_node{{cookiecutter.Node}} test_{{cookiecutter.Node}}Node.test
  test_{{cookiecutter.Node}}Node.cpp)
add_dependencies(test_node{{cookiecutter.Node}} node{{cookiecutter.Node}})
target_link_libraries(
  test_node{{cookiecutter.Node}} ${catkin_LIBRARIES} ${Boost_LIBRARIES}
  ${GTEST_LIBRARIES} {{cookiecutter.Node}}Process)
