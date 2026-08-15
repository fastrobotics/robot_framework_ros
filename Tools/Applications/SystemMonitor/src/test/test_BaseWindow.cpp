
#include <gtest/gtest.h>

#include <BaseWindow.hpp>
#include <Infrastructure/Logger.hpp>

using namespace fast::rf_ros::Tools::Applications::SystemMonitor;
class TestBaseWindow : public BaseWindow {
   public:
    TestBaseWindow(int16_t tab_order, int16_t mainwindow_height, uint16_t mainwindow_width)
        : BaseWindow("test_window", tab_order, 0.0, 0.0, 100.0, 100.0, mainwindow_height, mainwindow_width) {
        ScreenCoordinatePixel coord_pix =
            convertCoordinate(get_screen_coordinates_perc(), mainwindow_width, mainwindow_height);
        WINDOW* win =
            create_newwin(coord_pix.height_pix, coord_pix.width_pix, coord_pix.start_y_pix, coord_pix.start_x_pix);
        set_screen_coordinates_pix(coord_pix);
        set_window(win);
        wrefresh(win);
    }
    void new_ArmCommandMsg([[maybe_unused]] robot_framework_ros::arm_command msg) override {}
    void new_HeartbeatMsg([[maybe_unused]] robot_framework_ros::heartbeat msg) override {}

    void new_ReadyToArmMsg([[maybe_unused]] robot_framework_ros::ready_to_arm msg) override {}
    std::string pretty() {
        std::string str = "---Test Base Window---\n";
        str += BaseWindow::pretty();
        return str;
    }
    bool update(double current_time_sec) {
        bool status = BaseWindow::update(current_time_sec);
        if (status == false) {
            return false;
        }
        return update_window();
    }
    bool update_window() { return true; }
};
TEST(TestBaseWindow, BasicTests) {
    TestBaseWindow SUT(-1, 0, 0);
    ASSERT_NE(SUT.get_name(), "");
    ASSERT_FALSE(SUT.is_initialized());
    fast::rf::Logger::log_debug(SUT.pretty());
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
