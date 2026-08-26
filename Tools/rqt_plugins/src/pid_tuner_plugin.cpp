#include "robot_framework_ros/pid_tuner_plugin.h"

#include <robot_framework_ros/armedstate.h>
#include <ros/package.h>
#include <ros/ros.h>
#include <std_msgs/Float32.h>

#include <QFile>
#include <QVBoxLayout>
#include <QtUiTools/QUiLoader>
#include <chrono>
#include <fstream>
#include <iostream>
#include <pluginlib/class_list_macros.hpp>

namespace robot_framework_ros {
    PIDTunerPlugin::PIDTunerPlugin() : rqt_gui_cpp::Plugin() { setObjectName("PIDTunerPlugin"); }
    void PIDTunerPlugin::armed_command_Callback(const robot_framework_ros::arm_command::ConstPtr& t_msg) {
        robot_framework_ros::arm_command msg = *t_msg;
        latest_arm_command = msg;
    }
    void PIDTunerPlugin::sensor_Callback(const std_msgs::Float32::ConstPtr& t_msg) {
        sensor_data_rx_counter++;

        latest_sensor_ = t_msg->data;
        pid_controller.new_sensor_input(latest_sensor_, ros::Time::now().toSec());
    }
    void PIDTunerPlugin::saveConfigButtonPressed() {
        std::string out_file = std::string(std::getenv("HOME")) + "/var/log/output/pid_config.yaml";
        std::ofstream outFile(out_file);
        if (!outFile.is_open()) {
            std::cerr << "Error opening file: " << out_file << std::endl;
        }
        auto now = std::chrono::system_clock::now();

        // 2. Convert the time point to a C-style time_t
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        // 3. Convert time_t to local time structure (struct tm)
        std::tm local_tm = *std::localtime(&now_c);

        // 4. Use a stringstream and std::put_time to format the string
        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
        outFile << "# Auto-Generated on: " << oss.str() << std::endl;
        outFile << "max_output: " << std::to_string(max_output) << std::endl;
        outFile << "min_output: " << std::to_string(min_output) << std::endl;
        outFile << "K_P: " << std::to_string(K_P) << std::endl;
        outFile << "K_I: " << std::to_string(K_I) << std::endl;
        outFile << "K_D: " << std::to_string(K_D) << std::endl;
        outFile << "sensor_scale: " << std::to_string(sensor_scale_factor) << std::endl;
        outFile.close();
    }
    /*
    void PIDTunerPlugin::PGainScaleDiv2Pressed() {
        double max_offset = dial_P.max_value - K_P;
        dial_P.max_value = K_P + (max_offset / 2.0);
        double min_offset = (K_P - dial_P.min_value);
        dial_P.min_value = K_P - (min_offset / 2.0);
        dial_P.update();
        dial_P.set_dial((int)scale_value(K_P, dial_P.min_value, dial_P.max_value,
                                         (double)SmartDialContainer::min_tick_mark,
                                         (double)SmartDialContainer::max_tick_mark));
    }
                                         */
    /*
void PIDTunerPlugin::PGainScaleX2Pressed() {
double max_offset = dial_P.max_value - K_P;
dial_P.max_value = K_P + (max_offset * 2.0);
double min_offset = (K_P - dial_P.min_value);
dial_P.min_value = K_P - (min_offset * 2.0);
// dial_P.max_value = (dial_P.max_value - K_P) * 2.0;
// dial_P.min_value = (K_P - dial_P.min_value) * 2.0;
dial_P.update();
dial_P.set_dial((int)scale_value(K_P, dial_P.min_value, dial_P.max_value,
     (double)SmartDialContainer::min_tick_mark,
     (double)SmartDialContainer::max_tick_mark));
}
     */
    /*
    void PIDTunerPlugin::knobSetpointChanged(int value) {
        latest_setpoint_ =
            scale_value((double)value, (double)SmartDialContainer::min_tick_mark,
                        (double)SmartDialContainer::max_tick_mark, dial_set_point.min_value, dial_set_point.max_value);
        dial_set_point.set_value(latest_setpoint_);
    }
        */
    /*
    void PIDTunerPlugin::knobPGainChanged(int value) {
        K_P = scale_value((double)value, (double)SmartDialContainer::min_tick_mark,
                          (double)SmartDialContainer::max_tick_mark, dial_P.min_value, dial_P.max_value);
        dial_P.set_value(K_P);
    }
        */
    /*
 void PIDTunerPlugin::knobIGainChanged(int value) {
     K_I = scale_value((double)value, (double)SmartDialContainer::min_tick_mark,
                       (double)SmartDialContainer::max_tick_mark, dial_I.min_value, dial_I.max_value);
     dial_I.set_value(K_I);
 }
 void PIDTunerPlugin::knobDGainChanged(int value) {
     K_D = scale_value((double)value, (double)SmartDialContainer::min_tick_mark,
                       (double)SmartDialContainer::max_tick_mark, dial_D.min_value, dial_D.max_value);
     dial_D.set_value(K_D);
 }
 void PIDTunerPlugin::knobSensorScaleChanged(int value) {
     sensor_scale_factor = scale_value((double)value, (double)SmartDialContainer::min_tick_mark,
                                       (double)SmartDialContainer::max_tick_mark, dial_sensor_scale.min_value,
                                       dial_sensor_scale.max_value);
     dial_sensor_scale.set_value(sensor_scale_factor);
 }
     */
    void PIDTunerPlugin::initPlugin(qt_gui_cpp::PluginContext& context) {
        widget_ = new QWidget();
        std::string package_path = ros::package::getPath("robot_framework_ros");
        std::string ui_file_path = package_path + "/Tools/rqt_plugins/resource/pid_tuner_plugin.ui";
        pid_controller.init();
        QUiLoader loader;
        QFile file(QString::fromStdString(ui_file_path));
        if (!file.open(QFile::ReadOnly)) {
            throw std::runtime_error("Failed to open UI file at: " + ui_file_path);
        }

        // =========================================================================
        // FIX: Pass 'nullptr' instead of 'widget_'. Let the loader create it in isolation
        // =========================================================================
        QWidget* loaded_layout = loader.load(&file, nullptr);
        file.close();

        if (!loaded_layout) {
            throw std::runtime_error("Failed to parse and load .ui design components!");
        }

        QVBoxLayout* top_layout = new QVBoxLayout(widget_);
        top_layout->setContentsMargins(0, 0, 0, 0);
        top_layout->addWidget(loaded_layout);

        // Submit layout back to active rqt docking context tree
        context.addWidget(widget_);

        QWidget* container = loaded_layout->findChild<QWidget*>("chartContainer");
        if (container) {
            // Set up line data series structures
            setpoint_series_ = new QtCharts::QLineSeries();
            setpoint_series_->setName("Setpoint");
            sensor_series_ = new QtCharts::QLineSeries();
            sensor_series_->setName("Process Variable");
            output_series_ = new QtCharts::QLineSeries();
            output_series_->setName("Output");

            // Formulate chart sheet layout
            chart_ = new QtCharts::QChart();
            chart_->addSeries(setpoint_series_);
            chart_->addSeries(sensor_series_);
            chart_->addSeries(output_series_);
            chart_->setTitle("Live System Response Tracking");

            // Configure axes
            axis_x_ = new QtCharts::QValueAxis();
            axis_x_->setTitleText("Time (s)");
            chart_->addAxis(axis_x_, Qt::AlignBottom);
            setpoint_series_->attachAxis(axis_x_);
            sensor_series_->attachAxis(axis_x_);
            output_series_->attachAxis(axis_x_);

            axis_y_ = new QtCharts::QValueAxis();
            axis_y_->setTitleText("Value");
            axis_y_->setRange(-101.0, 101.0);  // Set expected robot limits
            chart_->addAxis(axis_y_, Qt::AlignLeft);
            setpoint_series_->attachAxis(axis_y_);
            sensor_series_->attachAxis(axis_y_);
            output_series_->attachAxis(axis_y_);

            // Explicitly create the view wrapper frame in pure C++
            QtCharts::QChartView* chart_view = new QtCharts::QChartView(chart_, container);
            chart_view->setRenderHint(QPainter::Antialiasing);

            // Give the container widget a layout so the chart expands to fill its boundaries
            QVBoxLayout* container_layout = new QVBoxLayout(container);
            container_layout->setContentsMargins(0, 0, 0, 0);
            container_layout->addWidget(chart_view);
        } else {
            throw std::runtime_error("Could not locate the 'chartContainer' layout placeholder box!");
        }

        // =========================================================================
        // NOTE: When looking for children, search 'loaded_layout' instead of 'widget_'
        // =========================================================================

        text_sensordata_rx_ = loaded_layout->findChild<QLabel*>("test_SensorData_rx");
        text_armedstate_ = loaded_layout->findChild<QLabel*>("text_ArmedState");
        button_saveConfig_ = loaded_layout->findChild<QPushButton*>("b_SaveConfig");
        if (button_saveConfig_) {
            connect(button_saveConfig_, &QPushButton::clicked, this, &PIDTunerPlugin::saveConfigButtonPressed);
        }
        // Initialize Set Point Dial
        /*
        dial_set_point.name = "Set Point Dial";
        dial_set_point.dial_ = loaded_layout->findChild<QDial*>("dial_Setpoint");

        dial_set_point.indicator_ = loaded_layout->findChild<QLabel*>("text_SetPointValue");
        dial_set_point.label_max_value_ = loaded_layout->findChild<QLabel*>("text_dialSetpoint_Max");
        dial_set_point.label_min_value_ = loaded_layout->findChild<QLabel*>("text_dialSetpoint_Min");
        latest_setpoint_ = 50.0;
        dial_set_point.max_value = 100.0;
        dial_set_point.min_value = -100.0;
        dial_set_point.set_value(latest_setpoint_);
        dial_set_point.set_dial((int)scale_value(latest_setpoint_, dial_set_point.min_value, dial_set_point.max_value,
                                                 (double)SmartDialContainer::min_tick_mark,
                                                 (double)SmartDialContainer::max_tick_mark));
        dial_set_point.update();

        if (dial_set_point.dial_) {
            connect(dial_set_point.dial_, &QDial::valueChanged, this, &PIDTunerPlugin::knobSetpointChanged);
        }

        // Initialize Sensor Scale Dial
        dial_sensor_scale.name = "Sensor Scale Dial";
        dial_sensor_scale.dial_ = loaded_layout->findChild<QDial*>("dial_SensorScaleTuner");

        dial_sensor_scale.indicator_ = loaded_layout->findChild<QLabel*>("text_SensorScaleValue");
        dial_sensor_scale.label_max_value_ = loaded_layout->findChild<QLabel*>("text_dialSensorScale_Max");
        dial_sensor_scale.label_min_value_ = loaded_layout->findChild<QLabel*>("text_dialSensorScale_Min");
        dial_sensor_scale.max_value = 100.0;
        dial_sensor_scale.min_value = -100.0;
        dial_sensor_scale.set_value(sensor_scale_factor);
        dial_sensor_scale.set_dial(
            (int)scale_value(sensor_scale_factor, dial_sensor_scale.min_value, dial_sensor_scale.max_value,
                             (double)SmartDialContainer::min_tick_mark, (double)SmartDialContainer::max_tick_mark));
        if (dial_sensor_scale.dial_) {
            connect(dial_sensor_scale.dial_, &QSlider::valueChanged, this, &PIDTunerPlugin::knobSensorScaleChanged);
        }

        if (dial_sensor_scale.is_initialized() == false) {
            throw std::runtime_error("Sensor Scaling Tuner not fully initialized!  Exiting.");
        }
        */
        // Initialize P Gain Dial
        dial_PGain_ = new SmartDial(this);
        if (!dial_PGain_->setupUi(widget_, "dialP", -1.5, 1.5)) {
            fast::rf::Logger::log_error("Help! UI components could not be found.");
            throw std::runtime_error("P Tuner not fully initialized!  Exiting.");
        }
        dial_PGain_->update();
        // dial_PGain_->dial_ = loaded_layout->findChild<QDial*>("dial_PGainTuner");
        // dial_PGain_->button_scale_X2_ = loaded_layout->findChild<QPushButton*>("button_PGain_ScaleX2");
        // QObject::connect(dial_PGain_->button_scale_X2_, &QPushButton::clicked, dial_PGain_,
        //                 &SmartDial::button_scale_X2_clicked);
        // if (dial_PGain_->dial_) {
        //    QObject::connect(dial_PGain_->dial_, &QDial::valueChanged, dial_PGain_, &SmartDial::dial_changed);
        //} else {
        //    fast::rf::Logger::log_error("Help!");
        //}
        /*
        dial_P.name = "P Gain Dial";
        // button_PGain_X2_ = loaded_layout->findChild<QPushButton*>("button_PGain_ScaleX2");
        button_PGain_Div2_ = loaded_layout->findChild<QPushButton*>("button_PGain_ScaleDiv2");
        dial_P.indicator_ = loaded_layout->findChild<QLabel*>("text_PGainValue");
        dial_P.label_max_value_ = loaded_layout->findChild<QLabel*>("text_dialPGain_Max");
        dial_P.label_min_value_ = loaded_layout->findChild<QLabel*>("text_dialPGain_Min");
        dial_P.max_value = 1.5;
        dial_P.min_value = -1.5;
        dial_P.set_value(K_P);
        dial_P.set_dial((int)scale_value(K_P, dial_P.min_value, dial_P.max_value,
                                         (double)SmartDialContainer::min_tick_mark,
                                         (double)SmartDialContainer::max_tick_mark));
        if (button_PGain_Div2_) {
            connect(button_PGain_Div2_, &QPushButton::clicked, this, &PIDTunerPlugin::PGainScaleDiv2Pressed);
        }

        if (dial_P.is_initialized() == false) {
            throw std::runtime_error("P Tuner not fully initialized!  Exiting.");
        }
            */

        // Initialize I Gain Dial
        /*
        dial_I.name = "I Gain Dial";
        dial_I.dial_ = loaded_layout->findChild<QDial*>("dial_IGainTuner");
        if (dial_I.dial_) {
            connect(dial_I.dial_, &QSlider::valueChanged, this, &PIDTunerPlugin::knobIGainChanged);
        }
        dial_I.indicator_ = loaded_layout->findChild<QLabel*>("text_IGainValue");
        dial_I.label_max_value_ = loaded_layout->findChild<QLabel*>("text_dialIGain_Max");
        dial_I.label_min_value_ = loaded_layout->findChild<QLabel*>("text_dialIGain_Min");
        dial_I.max_value = 0.1;
        dial_I.min_value = -0.1;
        dial_I.set_value(K_I);
        dial_I.set_dial((int)scale_value(K_I, dial_I.min_value, dial_I.max_value,
                                         (double)SmartDialContainer::min_tick_mark,
                                         (double)SmartDialContainer::max_tick_mark));
        if (dial_I.dial_) {
            connect(dial_I.dial_, &QSlider::valueChanged, this, &PIDTunerPlugin::knobIGainChanged);
        }
        if (dial_I.is_initialized() == false) {
            throw std::runtime_error("I Tuner not fully initialized!  Exiting.");
        }

        // Initialize D Gain Dial
        dial_D.name = "D Gain Dial";
        dial_D.dial_ = loaded_layout->findChild<QDial*>("dial_DGainTuner");
        if (dial_D.dial_) {
            connect(dial_D.dial_, &QSlider::valueChanged, this, &PIDTunerPlugin::knobDGainChanged);
        }
        dial_D.indicator_ = loaded_layout->findChild<QLabel*>("text_DGainValue");
        dial_D.label_max_value_ = loaded_layout->findChild<QLabel*>("text_dialDGain_Max");
        dial_D.label_min_value_ = loaded_layout->findChild<QLabel*>("text_dialDGain_Min");
        dial_D.max_value = 0.01;
        dial_D.min_value = -0.01;
        dial_D.set_value(K_D);
        dial_D.set_dial((int)scale_value(K_D, dial_D.min_value, dial_D.max_value,
                                         (double)SmartDialContainer::min_tick_mark,
                                         (double)SmartDialContainer::max_tick_mark));
        if (dial_D.dial_) {
            connect(dial_D.dial_, &QSlider::valueChanged, this, &PIDTunerPlugin::knobDGainChanged);
        }
        if (dial_D.is_initialized() == false) {
            throw std::runtime_error("D Tuner not fully initialized!  Exiting.");
        }
            */

        std::string setpoint_topic_name = "pidtuner_setpoint";
        setpoint_pub_ = nh_.advertise<std_msgs::Float32>(setpoint_topic_name, 1);
        latest_setpoint_ = 0.0;
        std::string sensor_topic_name = "pidtuner_sensor";
        sensor_sub_ = nh_.subscribe(sensor_topic_name, 100, &PIDTunerPlugin::sensor_Callback, this);

        std::string output_topic_name = "pidtuner_command";
        command_pub_ = nh_.advertise<std_msgs::Float32>(output_topic_name, 1);

        std::string arm_command_topic_name = "arm_command";
        armedstate_sub_ = nh_.subscribe(arm_command_topic_name, 100, &PIDTunerPlugin::armed_command_Callback, this);

        update_timer_ = new QTimer(this);
        connect(update_timer_, &QTimer::timeout, this, &PIDTunerPlugin::updateLoop);
        update_timer_->start(33);

        slow_loop_timer_ = new QTimer(this);
        connect(slow_loop_timer_, &QTimer::timeout, this, &PIDTunerPlugin::slowLoop);
        slow_loop_timer_->start(100);

        // initialize start time for graphing
        start_time_ = ros::Time::now();
    }
    void PIDTunerPlugin::updateLoop() {
        std::string str = "";

        switch (latest_arm_command.armed_state.state) {
            case robot_framework_ros::armedstate::DISARMED:
                str = "DISARMED";
                text_armedstate_->setStyleSheet("background-color: green; color: black;");
                break;
            case robot_framework_ros::armedstate::DISARMED_CANNOTARM:
                str = "DISARMED-CANNOTARM";
                text_armedstate_->setStyleSheet("background-color: red; color: black;");
                break;
            case robot_framework_ros::armedstate::ARMING:
                str = "ARMING";
                text_armedstate_->setStyleSheet("background-color: green; color: black;");
                break;
            case robot_framework_ros::armedstate::ARMED:
                str = "ARMED";
                text_armedstate_->setStyleSheet("background-color: blue; color: black;");
                break;
            default:
                str = "UNKNOWN";
                break;
        }
        text_armedstate_->setText(QString::fromStdString(str));

        fast::rf::NavigationSystem::Controller::PIDControllerConfig config;
        config.set_parameters(max_output, min_output, K_P, K_I, K_D, sensor_scale_factor);
        pid_controller.set_config(config);

        pid_controller.new_set_point(latest_setpoint_, ros::Time::now().toSec());

        auto output = pid_controller.get_output();
        if (output->is_new) {
            latest_output_ = output->command_value;
            std_msgs::Float32 command;
            command.data = latest_output_;
            command_pub_.publish(command);
        }
        // dial_set_point.update();
        // dial_sensor_scale.update();
        // dial_P.update();
        // dial_I.update();
        // dial_D.update();
        dial_PGain_->update();
        text_sensordata_rx_->setText("Sensor Data Rx: " + QString::number(sensor_data_rx_counter));
        std_msgs::Float32 setpoint;
        setpoint.data = latest_setpoint_;
        setpoint_pub_.publish(setpoint);

        updateGraphLoop();
    }
    void PIDTunerPlugin::updateGraphLoop() {
        double elapsed_time = (ros::Time::now() - start_time_).toSec();

        // Add points to the Qt lines
        setpoint_series_->append(elapsed_time, latest_setpoint_);
        sensor_series_->append(elapsed_time, latest_sensor_ * sensor_scale_factor);
        output_series_->append(elapsed_time, latest_output_);
        int maxPoints = 500;
        if (setpoint_series_->count() > maxPoints) {
            setpoint_series_->remove(0);
        }
        if (sensor_series_->count() > maxPoints) {
            sensor_series_->remove(0);
        }
        if (output_series_->count() > maxPoints) {
            output_series_->remove(0);
        }
        // Scroll the X axis window (shows last 10 seconds)
        if (elapsed_time > 10.0) {
            axis_x_->setRange(elapsed_time - 10.0, elapsed_time);
        } else {
            axis_x_->setRange(0, 10.0);
        }
    }
    void PIDTunerPlugin::slowLoop() {  // fast::rf::Logger::log_info(pid_controller.pretty());
    }
    void PIDTunerPlugin::shutdownPlugin() {
        setpoint_pub_.shutdown();
        sensor_sub_.shutdown();
        update_timer_->stop();
        nh_.shutdown();
    }
}  // namespace robot_framework_ros

PLUGINLIB_EXPORT_CLASS(robot_framework_ros::PIDTunerPlugin, rqt_gui_cpp::Plugin)
