
#include <gtest/gtest.h>

#include <Infrastructure/Logger.hpp>
#include <Windows/DiagnosticWindow.hpp>

using namespace fast::rf_ros::Tools::Applications::SystemMonitor;
TEST(DiagnosticWindow, BasicTests) {
    DiagnosticWindow SUT(-1, 100, 100);
    ASSERT_NE(SUT.getName(), "");
    ASSERT_FALSE(SUT.isInitialized());
    ASSERT_FALSE(SUT.update(0.0));
    fast::rf::Logger::logDebug(SUT.pretty());
}
TEST(DiagnosticWindow, NodeDiagnosticTests) {
    DiagnosticWindow SUT(-1, 100, 100);
    ASSERT_NE(SUT.getName(), "");
    ASSERT_FALSE(SUT.isInitialized());
    ASSERT_FALSE(SUT.update(0.0));
    fast::rf::Logger::logDebug(SUT.pretty());
    {
        robot_framework_ros::diagnostic node_diagnostic;
        node_diagnostic.NodeName = "node1";
        node_diagnostic.DiagnosticType = 1;
        node_diagnostic.Level = 2;
        SUT.newDiagnosticMsg(node_diagnostic);
    }
    {
        robot_framework_ros::diagnostic node_diagnostic;
        node_diagnostic.NodeName = "node1";
        node_diagnostic.DiagnosticType = 2;
        node_diagnostic.Level = 2;
        SUT.newDiagnosticMsg(node_diagnostic);
    }
    {
        robot_framework_ros::diagnostic node_diagnostic;
        node_diagnostic.NodeName = "node2";
        node_diagnostic.DiagnosticType = 2;
        node_diagnostic.Level = 2;
        SUT.newDiagnosticMsg(node_diagnostic);
    }

    ASSERT_EQ(SUT.get_node_diagnostic_monitors().size(), 2);
    ASSERT_EQ(SUT.get_node_diagnostic_monitors()["node1"].size(), 2);
    ASSERT_EQ(SUT.get_node_diagnostic_monitors()["node2"].size(), 1);

    ASSERT_FALSE(SUT.update(1.0));
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
