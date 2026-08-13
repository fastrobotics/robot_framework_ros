
#include <gtest/gtest.h>

#include <BaseWindow.hpp>
#include <Infrastructure/Logger.hpp>

using namespace fast::rf_ros::Tools::Applications::SystemMonitor;
class TestBaseWindow : public BaseWindow {
   public:
    TestBaseWindow() : BaseWindow("TestBaseWindow") {}
    std::string pretty() {
        std::string str = "---Test Base Window---\n";
        str += BaseWindow::pretty();
        return str;
    }
};
TEST(TestBaseWindow, BasicTests) {
    TestBaseWindow SUT;
    ASSERT_NE(SUT.get_name(), "");
    fast::rf::Logger::log_debug(SUT.pretty());
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}