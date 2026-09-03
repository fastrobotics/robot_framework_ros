#pragma once

#include <robot_framework_ros/arm_command.h>
#include <ros/ros.h>
#include <rqt_gui_cpp/plugin.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

#include <Controller/IController.hpp>
#include <Controller/PIDController/PIDController.hpp>
#include <ControllerTuner/PIDAutoTuner/PIDAutoTuner.hpp>
#include <QDial>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "SmartDial.h"

namespace Ui {
    class PIDTunerWidget;  // This must match the ObjectName of your main form in Qt Designer
}
namespace robot_framework_ros {
    class PIDTunerPlugin : public rqt_gui_cpp::Plugin {
        // Q_OBJECT
       public:
        PIDTunerPlugin();
        virtual void initPlugin(qt_gui_cpp::PluginContext& context) override;
        virtual void shutdownPlugin() override;
        void sensor_Callback(const std_msgs::Float32::ConstPtr& t_msg);
        void armed_command_Callback(const robot_framework_ros::arm_command::ConstPtr& t_msg);
       private slots:
        void autoTuneButtonPressed();
        void saveConfigButtonPressed();

        void updateGraphLoop();
        void updateLoop();
        void slowLoop();

       private:
        void switchToPidController();
        QWidget* widget_{nullptr};

        // Tuning Graph
        QtCharts::QLineSeries* setpoint_series_{nullptr};
        QtCharts::QLineSeries* sensor_series_{nullptr};
        QtCharts::QLineSeries* output_series_{nullptr};
        QtCharts::QChart* chart_{nullptr};
        QtCharts::QChartView* chart_view_{nullptr};
        QtCharts::QValueAxis* axis_x_{nullptr};
        QtCharts::QValueAxis* axis_y_{nullptr};

        // Smart Dial Controls
        SmartDial* dial_set_point_{nullptr};
        SmartDial* dial_sensor_scale_{nullptr};
        SmartDial* dial_PGain_{nullptr};
        SmartDial* dial_IGain_{nullptr};
        SmartDial* dial_DGain_{nullptr};

        // Status Controls
        QLabel* text_sensordata_rx_{nullptr};
        QLabel* text_armedstate_{nullptr};

        // Other Controls
        QPushButton* button_autoTune_{nullptr};
        QPushButton* button_saveConfig_{nullptr};

        // ROS
        ros::NodeHandle nh_;
        ros::Publisher setpoint_pub_;
        ros::Publisher command_pub_;
        ros::Subscriber sensor_sub_;
        ros::Subscriber armedstate_sub_;

        // Data Members
        QTimer* update_timer_{nullptr};
        QTimer* slow_loop_timer_{nullptr};
        ros::Time start_time_;

        double latest_setpoint_{0.0};
        double latest_sensor_{0.0};
        double latest_output_{0.0};
        uint64_t sensor_data_rx_counter{0};
        robot_framework_ros::arm_command latest_arm_command;

        // PID Tuning Values
        double max_output{100.0};
        double min_output{-100.0};
        double sensor_scale_factor{21.46};
        double K_P{1.396};
        double K_I{0.005};
        double K_D{};
        bool use_mock{false};
        bool autotune_running_{false};

        std::unique_ptr<fast::rf::NavigationSystem::Controller::IController> controller_;
    };

}  // namespace robot_framework_ros
