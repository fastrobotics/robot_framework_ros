#include "SystemMonitorNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <Windows/HeaderWindow.hpp>
#include <Windows/NodeInfoWindow.hpp>
#include <Windows/StatusWindow.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::Tools::Applications::SystemMonitor {

    SystemMonitorNode::SystemMonitorNode() {}
    SystemMonitorNode::~SystemMonitorNode() {
        windows.clear();
        endwin();
    }
    void SystemMonitorNode::arm_command_Callback(const robot_framework_ros::arm_command::ConstPtr& t_msg) {
        for (const auto& window : windows) {
            robot_framework_ros::arm_command msg = *t_msg;
            window.second->new_ArmCommandMsg(fast::rf_ros::utils::TranslateUtility::convert(msg));
        }
    }
    bool SystemMonitorNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Base Node!");
            return false;
        }

        status = init_screen();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Screen!");
            return false;
        }
        arm_command_sub = n->subscribe<robot_framework_ros::arm_command>(
            get_robotnamespace() + "/arm_command", 10, &SystemMonitorNode::arm_command_Callback, this);
        return true;
    }

    bool SystemMonitorNode::init_screen() {
        setlocale(LC_ALL, "");
        mousemask(ALL_MOUSE_EVENTS, NULL);
        initscr();
        timeout(0);
        keypad(stdscr, TRUE);
        clear();
        if (has_colors() == FALSE) {
            endwin();
            // logger->enable_consoleprint();
            fast::rf::Logger::log_error("Terminal does not support colors. Exiting.");
            return false;
        }
        curs_set(0);
        noecho();
        raw();

        start_color();
        init_color(COLOR_BLACK, 0, 0, 0);
        init_color(COLOR_GREEN, 0, 600, 0);
        init_color(10, 500, 0, 500);
        init_pair((uint8_t)Color::NO_COLOR, COLOR_WHITE, COLOR_BLACK);
        init_pair((uint8_t)Color::RED_COLOR, COLOR_WHITE, COLOR_RED);
        init_pair((uint8_t)Color::YELLOW_COLOR, COLOR_WHITE, COLOR_YELLOW);
        init_pair((uint8_t)Color::GREEN_COLOR, COLOR_WHITE, COLOR_GREEN);
        init_pair((uint8_t)Color::BLUE_COLOR, COLOR_WHITE, COLOR_BLUE);
        init_pair((uint8_t)Color::PURPLE_COLOR, COLOR_WHITE, 10);

        bool status = init_windows();
        if (status == false) {
            fast::rf::Logger::log_error("Unable to initialize Windows!");
            return false;
        }
        return true;
    }
    bool SystemMonitorNode::init_windows() {
        uint16_t mainwindow_width, mainwindow_height;
        getmaxyx(stdscr, mainwindow_height, mainwindow_width);
        {
            auto window = std::make_shared<HeaderWindow>(-1, mainwindow_height, mainwindow_width);
            windows[window->get_name()] = window;
            // highest_tab_index++;
        }
        {
            auto window = std::make_shared<NodeInfoWindow>(-1, mainwindow_height, mainwindow_width);
            windows[window->get_name()] = window;
            // highest_tab_index++;
        }
        {
            auto window = std::make_shared<StatusWindow>(-1, mainwindow_height, mainwindow_width);
            windows[window->get_name()] = window;
            // highest_tab_index++;
        }

        return true;
    }
    bool SystemMonitorNode::start() { return BaseNode::base_start(); }
    bool SystemMonitorNode::run_loop1() { return true; }
    bool SystemMonitorNode::run_loop2() { return true; }
    bool SystemMonitorNode::run_loop3() { return true; }
    bool SystemMonitorNode::run_100hz() { return true; }
    bool SystemMonitorNode::run_10hz() {
        int key_pressed = getch();
        if ((key_pressed == Key::KEY_q) || (key_pressed == Key::KEY_Q)) {
            kill_node = true;
        }
        // Update all Windows
        for (const auto& window : windows) {
            // window->new_command(window_commands);
            window.second->update(ros::Time::now().toSec());
        }
        flushinp();
        return true;
    }
    bool SystemMonitorNode::run_1hz() { return true; }
    bool SystemMonitorNode::run_01hz() {
        fast::rf::Logger::log_info(pretty());
        return true;
    }
    bool SystemMonitorNode::run_001hz() { return true; }

    void SystemMonitorNode::thread_loop() {
        while (kill_node == false) {
            ros::Duration(1.0).sleep();
        }
    }
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor

void signalinterrupt_handler(int sig) {
    fast::rf::Logger::log_warn("Killing SystemMonitorNode with Signal: " + std::to_string(sig));
    kill_node = true;
    exit(0);
}

using namespace fast::rf_ros::Tools::Applications::SystemMonitor;
int main(int argc, char** argv) {
    ros::init(argc, argv, "system_monitor");
    SystemMonitorNode* node = new SystemMonitorNode();
    signal(SIGINT, signalinterrupt_handler);
    signal(SIGTERM, signalinterrupt_handler);
    bool status = node->init();
    if (status == false) {
        // No practical way to unit test
        // LCOV_EXCL_START
        endwin();
        return EXIT_FAILURE;
        // LCOV_EXCL_STOP
    }
    status = node->start();
    if (status == false) {
        // No practical way to unit test
        // LCOV_EXCL_START
        return EXIT_FAILURE;
        // LCOV_EXCL_STOP
    }
    std::thread thread(&SystemMonitorNode::thread_loop, node);
    while ((status == true) and (kill_node == false)) {
        status = node->update();
    }
    thread.detach();
    delete node;
    return 0;
}