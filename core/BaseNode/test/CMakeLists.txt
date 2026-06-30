add_rostest_gtest(test_baseNode
    test_baseNode.test
    test_baseNode.cpp
)
add_dependencies(test_baseNode baseNode)
target_link_libraries(test_baseNode ${catkin_LIBRARIES}  ${Boost_LIBRARIES} ${GTEST_LIBRARIES} baseNode)