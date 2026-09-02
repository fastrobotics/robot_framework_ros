#include "SystemMonitorNode.hpp"

#include <Infrastructure/Logger.hpp>
#include <Windows/DiagnosticWindow.hpp>
#include <Windows/HeaderWindow.hpp>
#include <Windows/NodeInfoWindow.hpp>
#include <Windows/StatusWindow.hpp>
#include <robot_framework_ros/utils/TranslateUtility.hpp>
using namespace fast::rf_ros;
bool kill_node = false;
namespace fast::rf_ros::Tools::Applications::SystemMonitor {

    SystemMonitorNode::SystemMonitorNode() { m_filterList.insert(std::make_pair("rostopic", true)); }
    SystemMonitorNode::~SystemMonitorNode() {
        windows.clear();
        endwin();
    }
    void SystemMonitorNode::arm_command_Callback(const robot_framework_ros::arm_command::ConstPtr& t_msg) {
        robot_framework_ros::arm_command msg = *t_msg;
        for (const auto& window : windows) {
            window.second->newArmCommandMsg(msg);
        }
    }
    void SystemMonitorNode::heartbeat_Callback(const robot_framework_ros::heartbeat::ConstPtr& t_msg) {
        robot_framework_ros::heartbeat msg = *t_msg;
        for (const auto& window : windows) {
            window.second->newHeartbeatMsg(msg);
        }
    }
    void SystemMonitorNode::ready_to_arm_Callback(const robot_framework_ros::ready_to_arm::ConstPtr& t_msg) {
        robot_framework_ros::ready_to_arm msg = *t_msg;
        for (const auto& window : windows) {
            window.second->newReadyToArmMsg(msg);
        }
    }
    void SystemMonitorNode::diagnostic_Callback(const robot_framework_ros::diagnostic::ConstPtr& t_msg) {
        robot_framework_ros::diagnostic msg = *t_msg;
        for (const auto& window : windows) {
            window.second->newDiagnosticMsg(msg);
        }
    }
    bool SystemMonitorNode::init() {
        bool status = BaseNode::base_init();
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Base Node!");
            return false;
        }

        status = initScreen();
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Screen!");
            return false;
        }

        status = load_config();
        if (status == false) {
            fast::rf::Logger::logError("Unable to load config!");
            return false;
        }
        initBaseNodeDiagnostics(0, 0, 0);
        m_armCommandSub = n->subscribe<robot_framework_ros::arm_command>(
            get_robotnamespace() + "/arm_command", 10, &SystemMonitorNode::arm_command_Callback, this);
        return true;
    }
    bool SystemMonitorNode::load_config() {
        // No configuration
        return true;
    }
    bool SystemMonitorNode::initScreen() {
        setlocale(LC_ALL, "");
        mousemask(ALL_MOUSE_EVENTS, NULL);
        initscr();
        timeout(0);
        keypad(stdscr, TRUE);
        clear();
        if (has_colors() == FALSE) {
            endwin();
            // logger->enable_consoleprint();
            fast::rf::Logger::logError("Terminal does not support colors. Exiting.");
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

        bool status = initWindows();
        if (status == false) {
            fast::rf::Logger::logError("Unable to initialize Windows!");
            return false;
        }
        return true;
    }
    bool SystemMonitorNode::initWindows() {
        uint16_t mainwindowWidth, mainwindowHeight;
        getmaxyx(stdscr, mainwindowHeight, mainwindowWidth);
        {
            auto window = std::make_shared<HeaderWindow>(-1, mainwindowHeight, mainwindowWidth);
            windows[window->getName()] = window;
            // highest_tab_index++;
        }
        {
            auto window = std::make_shared<NodeInfoWindow>(-1, mainwindowHeight, mainwindowWidth);
            window->setFocus(true);
            windows[window->getName()] = window;

            // highest_tab_index++;
        }
        {
            auto window = std::make_shared<StatusWindow>(-1, mainwindowHeight, mainwindowWidth);
            windows[window->getName()] = window;
            // highest_tab_index++;
        }
        {
            auto window = std::make_shared<DiagnosticWindow>(-1, mainwindowHeight, mainwindowWidth);
            windows[window->getName()] = window;
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
            if (window.second->hasFocus()) {
                auto output = window.second->newKeyEvent(key_pressed);
                if (output.message.level > fast::rf::Level::NOTICE) {
                    fast::rf::Logger::logWarn(output.message.text);
                }
            }
            window.second->update(ros::Time::now().toSec());
            if (window.second->getName() == "node_info_window") {
                auto node_info_window = std::dynamic_pointer_cast<NodeInfoWindow>(window.second);
                m_selectedNode = node_info_window->get_m_selectedNode();
            }
            if (window.second->getName() == "diagnostic_window") {
                auto diagnostic_window = std::dynamic_pointer_cast<DiagnosticWindow>(window.second);
                diagnostic_window->setNodeToMonitor(m_selectedNode);
            }
        }
        flushinp();
        return true;
    }
    bool SystemMonitorNode::run_1hz() {
        bool status = rescanROSNetwork();
        if (status == false) {
            fast::rf::Logger::logWarn("Problem during ROS Scan!");
        }
        return status;
    }
    bool SystemMonitorNode::run_01hz() {
        fast::rf::Logger::logInfo(pretty());
        return true;
    }
    bool SystemMonitorNode::run_001hz() { return true; }

    void SystemMonitorNode::thread_loop() {
        while (ros::ok() && is_node_running) {
        }
    }
    void SystemMonitorNode::stop() { is_node_running = false; }
    bool SystemMonitorNode::updateMonitorList(std::vector<std::string> heartbeatList,
                                              std::vector<std::string>& newHeartbeatTopicsToSubscribe,
                                              std::vector<std::string> readyToArmList,
                                              std::vector<std::string>& newReadyToArmTopicsToSubscribe,
                                              std::vector<std::string> diagnosticList,
                                              std::vector<std::string>& newDiagnosticTopicsToSubscribe) {
        for (auto heartbeat : heartbeatList) {
            bool found_it = false;
            for (auto monitored_heartbeat : m_monitoredHeartbeatTopics) {
                if (monitored_heartbeat == heartbeat) {
                    found_it = true;
                    break;
                }
            }
            if (found_it == false) {
                m_monitoredHeartbeatTopics.push_back(heartbeat);
                newHeartbeatTopicsToSubscribe.push_back(heartbeat);
            }
        }
        for (auto readytoarm : readyToArmList) {
            bool found_it = false;
            for (auto monitored_readytoarm : m_monitoredReadyToArmTopics) {
                if (monitored_readytoarm == readytoarm) {
                    found_it = true;
                    break;
                }
            }
            if (found_it == false) {
                m_monitoredReadyToArmTopics.push_back(readytoarm);
                newReadyToArmTopicsToSubscribe.push_back(readytoarm);
            }
        }
        for (auto diagnostic : diagnosticList) {
            bool found_it = false;
            for (auto monitored_diagnostic : m_monitoredDiagnosticTopics) {
                if (monitored_diagnostic == diagnostic) {
                    found_it = true;
                    break;
                }
            }
            if (found_it == false) {
                m_monitoredDiagnosticTopics.push_back(diagnostic);
                newDiagnosticTopicsToSubscribe.push_back(diagnostic);
            }
        }
        return true;
    }
    bool SystemMonitorNode::rescanROSNetwork() {
        ros::V_string nodes;
        ros::master::getNodes(nodes);
        std::vector<std::string> node_list;
        for (ros::V_string::iterator it = nodes.begin(); it != nodes.end(); it++) {
            const std::string& nodeName = *it;
            bool addMe = true;
            if (nodeName.rfind(get_robotnamespace(), 0) != 0) {
                addMe = false;
            }
            if (addMe == true) {
                std::map<std::string, bool>::iterator filterIt = m_filterList.begin();
                while (filterIt != m_filterList.end()) {
                    if (filterIt->second == true) {
                        if (nodeName.find(filterIt->first) != std::string::npos) {
                            addMe = false;
                        }
                    }
                    filterIt++;
                }
            }
            if (addMe == true) {
                node_list.push_back(nodeName);
            }
        }
        ros::master::V_TopicInfo masterTopics;
        ros::master::getTopics(masterTopics);
        std::vector<std::string> heartbeatList;
        std::vector<std::string> readyToArmList;
        std::vector<std::string> diagnosticList;
        for (ros::master::V_TopicInfo::iterator it = masterTopics.begin(); it != masterTopics.end(); it++) {
            const ros::master::TopicInfo& info = *it;
            /*
            std::size_t found = info.name.find(BASE_NODE_NAME);
            if (found != std::string::npos) {
                continue;
            }
            */

            if (info.datatype == "robot_framework_ros/heartbeat") {
                if (info.name.rfind(get_robotnamespace(), 0) == 0) {
                    heartbeatList.push_back(info.name);
                }
            }
            if (info.datatype == "robot_framework_ros/ready_to_arm") {
                if (info.name.rfind(get_robotnamespace(), 0) == 0) {
                    readyToArmList.push_back(info.name);
                }
            }
            if (info.datatype == "robot_framework_ros/diagnostic") {
                if (info.name.rfind(get_robotnamespace(), 0) == 0) {
                    diagnosticList.push_back(info.name);
                }
            }
        }
        std::vector<std::string> newHeartbeatTopicsToSubscribe;
        std::vector<std::string> newReadyToArmTopicsToSubscribe;
        std::vector<std::string> newDiagnosticTopicsToSubscribe;
        bool status = updateMonitorList(heartbeatList, newHeartbeatTopicsToSubscribe, readyToArmList,
                                        newReadyToArmTopicsToSubscribe, diagnosticList, newDiagnosticTopicsToSubscribe);
        if (status == false) {
            fast::rf::Logger::logError("Unable to update Monitor List!");
            return false;
        }
        for (std::size_t i = 0; i < newHeartbeatTopicsToSubscribe.size(); ++i) {
            ros::Subscriber sub = n->subscribe<robot_framework_ros::heartbeat>(
                newHeartbeatTopicsToSubscribe.at(i), 50, &SystemMonitorNode::heartbeat_Callback, this);
            m_heartbeatSubs.push_back(sub);
        }
        for (std::size_t i = 0; i < newReadyToArmTopicsToSubscribe.size(); ++i) {
            ros::Subscriber sub = n->subscribe<robot_framework_ros::ready_to_arm>(
                newReadyToArmTopicsToSubscribe.at(i), 50, &SystemMonitorNode::ready_to_arm_Callback, this);
            m_readyToArmSubs.push_back(sub);
        }
        for (std::size_t i = 0; i < newDiagnosticTopicsToSubscribe.size(); ++i) {
            ros::Subscriber sub = n->subscribe<robot_framework_ros::diagnostic>(
                newDiagnosticTopicsToSubscribe.at(i), 50, &SystemMonitorNode::diagnostic_Callback, this);
            m_diagnosticSubs.push_back(sub);
        }
        return true;
    }

}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor

using namespace fast::rf_ros::Tools::Applications::SystemMonitor;
int main(int argc, char** argv) {
    ros::init(argc, argv, "system_monitor");
    auto node = std::make_unique<SystemMonitorNode>();
    if (!node->init()) {
        // LCOV_EXCL_START
        endwin();
        return EXIT_FAILURE;
        // LCOV_EXCL_STOP
    }

    if (!node->start()) {
        // LCOV_EXCL_START
        return EXIT_FAILURE;
        // LCOV_EXCL_STOP
    }
    std::thread thread(&SystemMonitorNode::thread_loop, node.get());
    bool status = true;
    while (ros::ok() && status && (kill_node == false)) {
        status = node->update();
        ros::spinOnce();
    }
    node->stop();
    if (thread.joinable()) {
        thread.join();
    }
    return 0;
}