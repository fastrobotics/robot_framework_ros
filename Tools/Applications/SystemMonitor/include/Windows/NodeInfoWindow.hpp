/**
 * @file NodeInfoWindow.hpp
 * @author David Gitz (davidgitz@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-08-13
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

#include <robot_framework_ros/nodestate.h>

#include <BaseWindow.hpp>
#include <mutex>
namespace fast::rf_ros::Tools::Applications::SystemMonitor {

    /**
     * @brief A Header Window
     *
     */
    class NodeInfoWindow : public BaseWindow {
       public:
        static constexpr double START_X_PERC =
            0.0; /*!< What percentage of the screen to put top left corner (X) of window. */
        static constexpr double START_Y_PERC =
            15.0; /*!< What percentage of the screen to put top left corner (Y) of window. */
        static constexpr double WIDTH_PERC = 66.0;  /*!< What percentage of the screen (Width) to draw the window. */
        static constexpr double HEIGHT_PERC = 60.0; /*!< What percentage of the screen (Height) to draw the window. */
        enum class NodeType { UNKNOWN = 0, FAST = 1, NON_FAST = 2 };
        enum class NodeFieldColumn {
            MARKER = 0,
            ID = 1,
            HOSTNAME = 2,
            NODENAME = 3,
            STATUS = 4,
            READY_TO_ARM = 5,
            RESTARTS = 6,
            PID = 7,
            CPU = 8,
            RAM = 9,
            RX = 10
        };
        /**
         * @brief Construct a new Header Window object
         *
         * @param tab_order
         * @param mainwindow_height
         * @param mainwindow_width
         */
        NodeInfoWindow(int16_t tab_order, int16_t mainwindow_height, uint16_t mainwindow_width)
            : BaseWindow("node_info_window", tab_order, START_X_PERC, START_Y_PERC, WIDTH_PERC, HEIGHT_PERC,
                         mainwindow_height, mainwindow_width) {
            ScreenCoordinatePixel coord_pix =
                convertCoordinate(get_screen_coordinates_perc(), mainwindow_width, mainwindow_height);
            supported_keys.push_back(KEY_UP);
            supported_keys.push_back(KEY_DOWN);
            node_window_fields.insert(std::pair<NodeFieldColumn, Field>(NodeFieldColumn::MARKER, Field("", 3)));
            node_window_fields.insert(std::pair<NodeFieldColumn, Field>(NodeFieldColumn::ID, Field("ID", 4)));
            // node_window_fields.insert(
            //     std::pair<NodeFieldColumn, Field>(NodeFieldColumn::HOSTNAME, Field(" Host ", 20)));
            node_window_fields.insert(
                std::pair<NodeFieldColumn, Field>(NodeFieldColumn::NODENAME, Field(" NodeName ", 30)));
            node_window_fields.insert(
                std::pair<NodeFieldColumn, Field>(NodeFieldColumn::STATUS, Field(" Status ", 14)));
            node_window_fields.insert(
                std::pair<NodeFieldColumn, Field>(NodeFieldColumn::READY_TO_ARM, Field(" Ready To Arm ", 15)));
            // node_window_fields.insert(
            //    std::pair<NodeFieldColumn, Field>(NodeFieldColumn::RESTARTS, Field(" Restarts ", 10)));
            // node_window_fields.insert(std::pair<NodeFieldColumn, Field>(NodeFieldColumn::PID, Field(" PID ", 8)));
            // node_window_fields.insert(std::pair<NodeFieldColumn, Field>(NodeFieldColumn::CPU, Field(" CPU(%) ",
            // 10))); node_window_fields.insert(std::pair<NodeFieldColumn, Field>(NodeFieldColumn::RAM, Field(" RAM(%)",
            // 10)));
            node_window_fields.insert(std::pair<NodeFieldColumn, Field>(NodeFieldColumn::RX, Field(" Rx ", 6)));
            WINDOW* win =
                create_newwin(coord_pix.height_pix, coord_pix.width_pix, coord_pix.start_y_pix, coord_pix.start_x_pix);
            set_screen_coordinates_pix(coord_pix);
            set_window(win);

            std::string header = get_window_header();
            mvwprintw(win, 1, 1, header.c_str());
            std::string dashed(get_screen_coordinates_pixel().width_pix - 2, '-');
            mvwprintw(win, 2, 1, dashed.c_str());
            wrefresh(win);
        }
        KeyEventContainer new_keyevent(int key) override;
        void new_ArmCommandMsg([[maybe_unused]] robot_framework_ros::arm_command msg) override {}

        void new_HeartbeatMsg(robot_framework_ros::heartbeat msg) override;

        void new_ReadyToArmMsg(robot_framework_ros::ready_to_arm msg) override;

        void new_DiagnosticMsg([[maybe_unused]] robot_framework_ros::diagnostic msg) override {}

        std::string get_selected_node() { return selected_node; }

        /**
         * @brief Human readable string
         *
         * @return std::string
         */
        std::string pretty() override;

        /**
         * @brief Update at a periodic rate
         *
         * @param current_time_sec
         * @return true
         * @return false
         */
        bool update(double current_time_sec) override;

       protected:
        bool update_window();

       private:
        struct NodeData {
            NodeData() = default;
            NodeData(int16_t _id, NodeType _type, std::string _host_device, std::string _base_node_name,
                     std::string _node_name)
                : id(_id),
                  type(_type),
                  pid(0),
                  host_device(_host_device),
                  base_node_name(_base_node_name),
                  node_name(_node_name),
                  cpu_used_perc(0.0),
                  last_heartbeat(0.0),
                  last_heartbeat_delta(0.0),
                  restart_count(0) {
                state.state = robot_framework_ros::nodestate::STATE_STARTING;
            }
            bool initialized;
            uint16_t id;
            robot_framework_ros::nodestate state;
            NodeType type;
            uint16_t pid;
            std::string host_device;
            std::string base_node_name;
            std::string node_name;
            std::string ready_to_arm{"UNKNOWN"};
            double cpu_used_perc;
            double mem_used_perc;
            double last_heartbeat;
            double last_heartbeat_delta;
            uint64_t restart_count;
        };
        bool insertNode(NodeType node_type, std::string device, std::string base_node_name, std::string node_name);
        std::string get_window_header();
        std::string get_node_info(NodeData node, bool selected);

        int previous_key{-1};
        std::mutex node_list_mutex;
        std::map<NodeFieldColumn, Field> node_window_fields;
        std::string selected_node{""};
        std::map<std::string, NodeData> nodes;
    };
}  // namespace fast::rf_ros::Tools::Applications::SystemMonitor