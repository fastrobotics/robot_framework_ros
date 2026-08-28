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
        double current_time_sec = ros::Time::now().toSec();
        sensor_data_rx_counter++;

        latest_sensor_ = t_msg->data;
        controller_->new_sensor_input(latest_sensor_, current_time_sec);
    }
    void PIDTunerPlugin::autoTuneButtonPressed() {
        controller_->clear();
        if (use_mock == true) {
            throw std::runtime_error("No Mock supported!");
        } else {
            auto auto_tuner = std::make_unique<fast::rf::NavigationSystem::Controller::ControllerTuner::PIDAutoTuner>();
            fast::rf::NavigationSystem::Controller::ControllerTuner::PIDAutoTunerConfig config;

            config.set_parameters(50.0,                  // maximum output
                                  -50.0,                 // minimum output
                                  K_P,                   // initial K_P
                                  K_I,                   // initial K_I
                                  K_D,                   // initial K_D
                                  sensor_scale_factor);  // sensor scale factor

            config.set_tuning_parameters(2.0,   // output step applied to the system
                                         1.0,   // generated setpoint step
                                         1.0,   // settle time in seconds
                                         3.0,   // response timeout in seconds
                                         0.5,   // minimum measured response
                                         0.05,  // acceptable maximum tracking error
                                         1.0,   // evaluation duration in seconds
                                         3);    // maximum candidate-gain iterations

            config.set_algorithm(fast::rf::NavigationSystem::Controller::ControllerTuner::PIDAutoTuner::
                                     PIDAutoTuningAlgorithm::IMC_LAMBDA);

            if (!tuner.set_config(config)) {
                throw std::runtime_error("Unable to set Tuning Config!  Aborting.");
            }
            if (!tuner.init()) {
                throw std::runtime_error("Unable to initialize Tuner!  Aborting.");
            }
            if (!tuner.start_tuning()) {
                throw std::runtime_error("Unable to start Tuning!  Aborting.");
            }
            controller_ = std::move(auto_tuner);
        }
        autotune_running_ = true;
    }
    void PIDTunerPlugin::switchToPidController() {
        if (use_mock) {
        } else {
            if (dynamic_cast<fast::rf::NavigationSystem::Controller::ControllerTuner::PIDAutoTuner*>(
                    controller_.get()) == nullptr) {
                fast::rf::Logger::log_warn("Not an Auto-Tuner!");
                return;
            }
        }

        auto pid = std::make_unique<fast::rf::NavigationSystem::Controller::PIDController>();
        fast::rf::NavigationSystem::Controller::PIDControllerConfig config;
        config.set_parameters(max_output, min_output, K_P, K_I, K_D, sensor_scale_factor);

        if (!pid->set_config(config)) {
            throw std::runtime_error("Unable to configure PID controller");
        }
        pid->init();
        controller_ = std::move(pid);
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
    void PIDTunerPlugin::initPlugin(qt_gui_cpp::PluginContext& context) {
        widget_ = new QWidget();
        std::string package_path = ros::package::getPath("robot_framework_ros");
        std::string ui_file_path = package_path + "/Tools/rqt_plugins/resource/pid_tuner_plugin.ui";

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
        button_autoTune_ = loaded_layout->findChild<QPushButton*>("b_AutoTune");
        if (button_autoTune_) {
            connect(button_autoTune_, &QPushButton::clicked, this, &PIDTunerPlugin::autoTuneButtonPressed);
        }
        // Initialize Set Point Dial
        dial_set_point_ = new SmartDial(this);
        if (!dial_set_point_->setupUi(widget_, "dialSetPoint", -100.0, 100.0)) {
            std::string str = "Set Point Dial not fully initialized!  Exiting.";
            fast::rf::Logger::log_error(str);
            throw std::runtime_error(str);
        }
        dial_set_point_->set_value(latest_setpoint_);
        dial_set_point_->update();
        // Initialize Sensor Scale Dial
        dial_sensor_scale_ = new SmartDial(this);
        if (!dial_sensor_scale_->setupUi(widget_, "dialSensorScale", -100.0, 100.0)) {
            std::string str = "Sensor Scale Dial not fully initialized!  Exiting.";
            fast::rf::Logger::log_error(str);
            throw std::runtime_error(str);
        }
        dial_sensor_scale_->set_value(sensor_scale_factor);
        dial_sensor_scale_->update();
        // Initialize P Gain Dial
        dial_PGain_ = new SmartDial(this);
        if (!dial_PGain_->setupUi(widget_, "dialP", -1.5, 1.5)) {
            std::string str = "P Tuner not fully initialized!  Exiting.";
            fast::rf::Logger::log_error(str);
            throw std::runtime_error(str);
        }
        dial_PGain_->set_value(K_P);
        dial_PGain_->update();
        // Initialize I Gain Dial
        dial_IGain_ = new SmartDial(this);
        if (!dial_IGain_->setupUi(widget_, "dialI", -0.0, 0.4)) {
            std::string str = "I Tuner not fully initialized!  Exiting.";
            fast::rf::Logger::log_error(str);
            throw std::runtime_error(str);
        }
        dial_IGain_->set_value(K_I);
        dial_IGain_->update();
        // Initialize D Gain Dial
        dial_DGain_ = new SmartDial(this);
        if (!dial_DGain_->setupUi(widget_, "dialD", -0.01, 0.01)) {
            std::string str = "D Tuner not fully initialized!  Exiting.";
            fast::rf::Logger::log_error(str);
            throw std::runtime_error(str);
        }
        dial_DGain_->set_value(K_D);
        dial_DGain_->update();

        controller_ = std::make_unique<fast::rf::NavigationSystem::Controller::PIDController>();
        fast::rf::NavigationSystem::Controller::PIDControllerConfig config;
        config.set_parameters(max_output, min_output, K_P, K_I, K_D, sensor_scale_factor);
        auto* pid_controller = dynamic_cast<fast::rf::NavigationSystem::Controller::PIDController*>(controller_.get());
        if (pid_controller == nullptr || !pid_controller->set_config(config)) {
            throw std::runtime_error("Not able to set config for PID Controller.");
        }

        controller_->init();

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
        slow_loop_timer_->start(1000);

        // initialize start time for graphing
        start_time_ = ros::Time::now();
    }
    void PIDTunerPlugin::updateLoop() {
        double time_stamp_sec = ros::Time::now().toSec();
        controller_->update(time_stamp_sec);
        auto output = controller_->get_output();
        if (output->is_new) {
            latest_output_ = output->command_value;
            std_msgs::Float32 command;
            command.data = latest_output_;
            command_pub_.publish(command);
        }
        if (autotune_running_ == false) {
            controller_->new_set_point(latest_setpoint_, time_stamp_sec);
            fast::rf::NavigationSystem::Controller::PIDControllerConfig config;
            config.set_parameters(max_output, min_output, K_P, K_I, K_D, sensor_scale_factor);
            auto* pid_controller =
                dynamic_cast<fast::rf::NavigationSystem::Controller::PIDController*>(controller_.get());
            if (pid_controller == nullptr || !pid_controller->set_config(config)) {
                throw std::runtime_error("Not able to set config for PID Controller!");
            }
        }
        if (autotune_running_ == true) {
            auto* tuner_output =
                dynamic_cast<fast::rf::NavigationSystem::Controller::ControllerTuner::PIDAutoTuner*>(output);
            if (tuner_output != nullptr) {
                latest_setpoint_ = tuner_output->set_point;
                K_P = tuner_output->K_P;
                K_I = tuner_output->K_I;
                K_D = tuner_output->K_D;
                dial_set_point_->set_value(latest_setpoint_);
                dial_PGain_->set_value(K_P);
                dial_IGain_->set_value(K_I);
                dial_DGain_->set_value(K_D);
                switch (tuner_output->state) {
                    case fast::rf::NavigationSystem::Controller::ControllerTuner::AutoTunerState::IDLE:
                        button_autoTune_->setStyleSheet("background-color: gray; color: black;");
                        break;

                    case fast::rf::NavigationSystem::Controller::ControllerTuner::AutoTunerState::TUNING:
                        button_autoTune_->setStyleSheet("background-color: yellow; color: black;");
                        break;

                    case fast::rf::NavigationSystem::Controller::ControllerTuner::AutoTunerState::COMPLETE:
                        fast::rf::Logger::log_notice("Auto-Tuner tuned System!");
                        button_autoTune_->setStyleSheet("background-color: green; color: black;");
                        latest_setpoint_ = 0.0;
                        latest_output_ = 0.0;
                        dial_set_point_->set_value(latest_setpoint_);
                        controller_->clear();
                        autotune_running_ = false;
                        switchToPidController();
                        break;
                    case fast::rf::NavigationSystem::Controller::ControllerTuner::AutoTunerState::FAILED:
                        fast::rf::Logger::log_error("Auto-Tuner unable to tune System!");
                        button_autoTune_->setStyleSheet("background-color: red; color: black;");
                        latest_setpoint_ = 0.0;
                        latest_output_ = 0.0;
                        dial_set_point_->set_value(latest_setpoint_);
                        controller_->clear();
                        autotune_running_ = false;
                        switchToPidController();
                        break;
                    default:
                        break;
                }
            } else {
                fast::rf::Logger::log_error("Help");
            }
        } else {
            latest_setpoint_ = dial_set_point_->get_value();
            sensor_scale_factor = dial_sensor_scale_->get_value();
            K_P = dial_PGain_->get_value();
            K_I = dial_IGain_->get_value();
            K_D = dial_DGain_->get_value();
        }
        dial_set_point_->update();
        dial_sensor_scale_->update();
        dial_PGain_->update();
        dial_IGain_->update();
        dial_DGain_->update();

        std::string str = "";
        bool controller_clear = true;
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
                controller_clear = false;
                str = "ARMED";
                text_armedstate_->setStyleSheet("background-color: blue; color: black;");
                break;
            default:
                str = "UNKNOWN";
                break;
        }
        if ((controller_clear == true) && (autotune_running_ == false)) {
            controller_->clear();
        }
        text_armedstate_->setText(QString::fromStdString(str));

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
    void PIDTunerPlugin::slowLoop() { fast::rf::Logger::log_info(controller_->pretty()); }
    void PIDTunerPlugin::shutdownPlugin() {
        setpoint_pub_.shutdown();
        sensor_sub_.shutdown();
        update_timer_->stop();
        nh_.shutdown();
    }
}  // namespace robot_framework_ros

PLUGINLIB_EXPORT_CLASS(robot_framework_ros::PIDTunerPlugin, rqt_gui_cpp::Plugin)
