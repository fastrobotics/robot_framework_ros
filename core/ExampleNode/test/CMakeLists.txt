add_rostest_gtest(test_exampleNode
    test_ExampleNode.test
    test_ExampleNode.cpp
)
add_dependencies(test_exampleNode example_node)
target_link_libraries(test_exampleNode ${catkin_LIBRARIES}  ${Boost_LIBRARIES} ${GTEST_LIBRARIES})