#pragma once

#include <robot_framework_ros/arm_command.h>
#include <ros/ros.h>
#include <rqt_gui_cpp/plugin.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

#include <Controller/PIDController/PIDController.hpp>
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

namespace Ui {
    class PIDTunerWidget;  // This must match the ObjectName of your main form in Qt Designer
}
namespace robot_framework_ros {

    class PIDTunerPlugin : public rqt_gui_cpp::Plugin {
        // Q_OBJECT
       public:
        struct SmartDialContainer {
            static constexpr int max_tick_mark = 1024;
            static constexpr int min_tick_mark = -1024;
            std::string name{""};
            double max_value{0.0};
            double min_value{0.0};
            double current_value{0.0};
            bool is_initialized() {
                if (name == "") {
                    fast::rf::Logger::log_error("Dial name is empty!");
                    return false;
                }
                if (dial_ == nullptr) {
                    fast::rf::Logger::log_error("Dial: " + name + " dial object is null!");
                    return false;
                }
                if (indicator_ == nullptr) {
                    fast::rf::Logger::log_error("Dial: " + name + " indicator object is null!");
                    return false;
                }
                if (label_max_value_ == nullptr) {
                    fast::rf::Logger::log_error("Dial: " + name + " max value object is null!");
                    return false;
                }
                if (label_min_value_ == nullptr) {
                    fast::rf::Logger::log_error("Dial: " + name + " min value object is null!");
                    return false;
                }
                return true;
            }
            void set_dial(int value) { dial_->setValue(value); }
            void set_value(double value) { latest_value = value; }
            void update() {
                label_max_value_->setText(QString::number(max_value));
                label_min_value_->setText(QString::number(min_value));
                indicator_->setText(QString::number(latest_value));
            }
            QDial* dial_;
            QLabel* indicator_;
            QLabel* label_max_value_;
            QLabel* label_min_value_;
            double latest_value;
            SmartDialContainer()
                : dial_{nullptr}, indicator_{nullptr}, label_max_value_{nullptr}, label_min_value_{nullptr} {}
        };
        PIDTunerPlugin();
        virtual void initPlugin(qt_gui_cpp::PluginContext& context) override;
        virtual void shutdownPlugin() override;
        void sensor_Callback(const std_msgs::Float32::ConstPtr& t_msg);
        void armed_command_Callback(const robot_framework_ros::arm_command::ConstPtr& t_msg);
       private slots:
        void knobSetpointChanged(int value);
        void onSliderMoved(int value);
        void knobSensorScaleChanged(int value);
        void knobPGainChanged(int value);
        void knobIGainChanged(int value);
        void knobDGainChanged(int value);
        void saveConfigButtonPressed();
        void PGainScaleX2Pressed();
        void PGainScaleDiv2Pressed();

        void updateGraphLoop();
        void updateLoop();
        void slowLoop();

       private:
        double scale_value(double input, double min_input, double max_input, double min_output, double max_output);
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
        SmartDialContainer dial_set_point;
        SmartDialContainer dial_sensor_scale;
        SmartDialContainer dial_P;

        QPushButton* button_PGain_X2_{nullptr};
        QPushButton* button_PGain_Div2_{nullptr};
        SmartDialContainer dial_I;
        SmartDialContainer dial_D;

        // Status Controls
        QLabel* text_sensordata_rx_{nullptr};
        QLabel* text_armedstate_{nullptr};

        // Other Controls
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
        double sensor_scale_factor{31.4159};  // 5 full rotations per second is 100% --> 31.4
        double K_P{0.75};
        double K_I{0.0};
        double K_D{0.0};

        fast::rf::NavigationSystem::Controller::PIDController pid_controller;
    };

}  // namespace robot_framework_ros
