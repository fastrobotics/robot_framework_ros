#include "SystemMonitorNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <Windows/HeaderWindow.hpp>
#include <Windows/NodeInfoWindow.hpp>
#include <Windows/StatusWindow.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
bool kill_node = false;
using namespace fast::rf_ros;
namespace fast::rf_ros::Tools::Applications::SystemMonitor {

    SystemMonitorNode::SystemMonitorNode() { filter_list.insert(std::make_pair("rostopic", true)); }
    SystemMonitorNode::~SystemMonitorNode() {
        windows.clear();
        endwin();
    }
    void SystemMonitorNode::arm_command_Callback(const robot_framework_ros::arm_command::ConstPtr& t_msg) {
        robot_framework_ros::arm_command msg = *t_msg;
        for (const auto& window : windows) {
            window.second->new_ArmCommandMsg(msg);
        }
    }
    void SystemMonitorNode::heartbeat_Callback(const robot_framework_ros::heartbeat::ConstPtr& t_msg) {
        robot_framework_ros::heartbeat msg = *t_msg;
        for (const auto& window : windows) {
            window.second->new_HeartbeatMsg(msg);
        }
    }
    void SystemMonitorNode::ready_to_arm_Callback(const robot_framework_ros::ready_to_arm::ConstPtr& t_msg) {
        robot_framework_ros::ready_to_arm msg = *t_msg;
        for (const auto& window : windows) {
            window.second->new_ReadyToArmMsg(msg);
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
        init_pair((uint8_t)Color::WHITE_COLOR, COLOR_BLACK, COLOR_WHITE);
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
            window->set_focus(true);
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
            if (window.second->has_focus()) {
                auto output = window.second->new_keyevent(key_pressed);
                if (output.message.level > fast::rf::Level::NOTICE) {
                    fast::rf::Logger::log_warn(output.message.text);
                }
            }
            window.second->update(ros::Time::now().toSec());
            if (window.second->get_name() == "node_info_window") {
                auto node_info_window = std::dynamic_pointer_cast<NodeInfoWindow>(window.second);
                std::string selected_node = node_info_window->get_selected_node();
                // Do something with this selected_node during AB#1821
            }
        }
        flushinp();
        return true;
    }
    bool SystemMonitorNode::run_1hz() {
        bool status = rescan_rosnetwork();
        if (status == false) {
            fast::rf::Logger::log_warn("Problem during ROS Scan!");
        }
        return status;
    }
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
    bool SystemMonitorNode::update_monitorlist(std::vector<std::string> heartbeat_list,
                                               std::vector<std::string>& new_heartbeat_topics_to_subscribe,
                                               std::vector<std::string> readytoarm_list,
                                               std::vector<std::string>& new_readytoarm_topics_to_subscribe) {
        for (auto heartbeat : heartbeat_list) {
            bool found_it = false;
            for (auto monitored_heartbeat : monitored_heartbeat_topics) {
                if (monitored_heartbeat == heartbeat) {
                    found_it = true;
                    break;
                }
            }
            if (found_it == false) {
                monitored_heartbeat_topics.push_back(heartbeat);
                new_heartbeat_topics_to_subscribe.push_back(heartbeat);
            }
        }
        for (auto readytoarm : readytoarm_list) {
            bool found_it = false;
            for (auto monitored_readytoarm : monitored_readytoarm_topics) {
                if (monitored_readytoarm == readytoarm) {
                    found_it = true;
                    break;
                }
            }
            if (found_it == false) {
                monitored_readytoarm_topics.push_back(readytoarm);
                new_readytoarm_topics_to_subscribe.push_back(readytoarm);
            }
        }
        return true;
    }
    bool SystemMonitorNode::rescan_rosnetwork() {
        ros::V_string nodes;
        ros::master::getNodes(nodes);
        std::vector<std::string> node_list;
        for (ros::V_string::iterator it = nodes.begin(); it != nodes.end(); it++) {
            const std::string& _node_name = *it;
            /*
            std::size_t found = _node_name.find(BASE_NODE_NAME);
            if (found != std::string::npos) {
                continue;
            }
                */
            bool add_me = true;
            if (_node_name.rfind(get_robotnamespace(), 0) != 0) {
                add_me = false;
            }
            if (add_me == true) {
                std::map<std::string, bool>::iterator filter_it = filter_list.begin();
                while (filter_it != filter_list.end()) {
                    if (filter_it->second == true) {
                        if (_node_name.find(filter_it->first) != std::string::npos) {
                            add_me = false;
                        }
                    }
                    filter_it++;
                }
            }
            if (add_me == true) {
                node_list.push_back(_node_name);
            }
        }
        ros::master::V_TopicInfo master_topics;
        ros::master::getTopics(master_topics);
        std::vector<std::string> heartbeat_list;
        std::vector<std::string> readytoarm_list;
        for (ros::master::V_TopicInfo::iterator it = master_topics.begin(); it != master_topics.end(); it++) {
            const ros::master::TopicInfo& info = *it;
            /*
            std::size_t found = info.name.find(BASE_NODE_NAME);
            if (found != std::string::npos) {
                continue;
            }
            */

            if (info.datatype == "robot_framework_ros/heartbeat") {
                if (info.name.rfind(get_robotnamespace(), 0) == 0) {
                    heartbeat_list.push_back(info.name);
                }
            }
            if (info.datatype == "robot_framework_ros/ready_to_arm") {
                if (info.name.rfind(get_robotnamespace(), 0) == 0) {
                    readytoarm_list.push_back(info.name);
                }
            }
        }
        std::vector<std::string> new_heartbeat_topics_to_subscribe;
        std::vector<std::string> new_readytoarm_topics_to_subscribe;
        bool status = update_monitorlist(heartbeat_list, new_heartbeat_topics_to_subscribe, readytoarm_list,
                                         new_readytoarm_topics_to_subscribe);
        if (status == false) {
            fast::rf::Logger::log_error("Unable to update Monitor List!");
            return false;
        }
        for (std::size_t i = 0; i < new_heartbeat_topics_to_subscribe.size(); ++i) {
            ros::Subscriber sub = n->subscribe<robot_framework_ros::heartbeat>(
                new_heartbeat_topics_to_subscribe.at(i), 50, &SystemMonitorNode::heartbeat_Callback, this);
            heartbeat_subs.push_back(sub);
        }
        for (std::size_t i = 0; i < new_readytoarm_topics_to_subscribe.size(); ++i) {
            ros::Subscriber sub = n->subscribe<robot_framework_ros::ready_to_arm>(
                new_readytoarm_topics_to_subscribe.at(i), 50, &SystemMonitorNode::ready_to_arm_Callback, this);
            readytoarm_subs.push_back(sub);
        }
        return true;
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