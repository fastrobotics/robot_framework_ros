#include "robot_framework_ros/pid_tuner_plugin.h"

#include <ros/package.h>
#include <ros/ros.h>
#include <std_msgs/Float32.h>

#include <QFile>
#include <QVBoxLayout>
#include <QtUiTools/QUiLoader>
#include <pluginlib/class_list_macros.hpp>

namespace robot_framework_ros {
    PIDTunerPlugin::PIDTunerPlugin() : rqt_gui_cpp::Plugin() { setObjectName("PIDTunerPlugin"); }
    // void PIDTunerPlugin::setpoint_Callback(const std_msgs::Float32::ConstPtr& t_msg) { latest_setpoint_ =
    // t_msg->data; }
    void PIDTunerPlugin::sensor_Callback(const std_msgs::Float32::ConstPtr& t_msg) {
        sensor_data_rx_counter++;
        text_sensordata_rx_->setText(QString::number(sensor_data_rx_counter));
        latest_sensor_ = t_msg->data;
        pid_controller.new_sensor_input(latest_sensor_, ros::Time::now().toSec());
    }
    void PIDTunerPlugin::knobSetpointChanged(int value) {
        latest_setpoint_ = scale_knob_value(value, dial_set_point.max_value, dial_set_point.min_value);
        dial_set_point.set_value(latest_setpoint_);
    }
    void PIDTunerPlugin::knobPGainChanged(int value) {
        K_P = scale_knob_value(value, dial_P.max_value, dial_P.min_value);
        dial_P.set_value(K_P);
    }
    void PIDTunerPlugin::knobSensorScaleChanged(int value) {
        sensor_scale_factor = scale_knob_value(value, dial_sensor_scale.max_value, dial_sensor_scale.min_value);
        dial_sensor_scale.set_value(sensor_scale_factor);
    }
    void PIDTunerPlugin::initPlugin(qt_gui_cpp::PluginContext& context) {
        widget_ = new QWidget();
        std::string package_path = ros::package::getPath("robot_framework_ros");
        std::string ui_file_path = package_path + "/Tools/rqt_plugins/resource/pid_tuner_plugin.ui";
        ROS_WARN("Starting!");
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
        // Initialize Set Point Dial

        dial_set_point.name = "Set Point Dial";
        dial_set_point.dial_ = loaded_layout->findChild<QDial*>("dial_Setpoint");
        if (dial_set_point.dial_) {
            connect(dial_set_point.dial_, &QDial::valueChanged, this, &PIDTunerPlugin::knobSetpointChanged);
        }
        dial_set_point.indicator_ = loaded_layout->findChild<QLabel*>("text_SetPointValue");
        dial_set_point.label_max_value_ = loaded_layout->findChild<QLabel*>("text_dialSetpoint_Max");
        dial_set_point.label_min_value_ = loaded_layout->findChild<QLabel*>("text_dialSetpoint_Min");
        dial_set_point.max_value = 100.0;
        dial_set_point.min_value = -100.0;
        dial_set_point.set_value(latest_setpoint_);

        // Initialize Sensor Scale Dial
        dial_sensor_scale.name = "Sensor Scale Dial";
        dial_sensor_scale.dial_ = loaded_layout->findChild<QDial*>("dial_SensorScaleTuner");
        if (dial_sensor_scale.dial_) {
            connect(dial_sensor_scale.dial_, &QSlider::valueChanged, this, &PIDTunerPlugin::knobSensorScaleChanged);
        }
        dial_sensor_scale.indicator_ = loaded_layout->findChild<QLabel*>("text_SensorScaleValue");
        dial_sensor_scale.label_max_value_ = loaded_layout->findChild<QLabel*>("text_dialSensorScale_Max");
        dial_sensor_scale.label_min_value_ = loaded_layout->findChild<QLabel*>("text_dialSensorScale_Min");
        dial_sensor_scale.max_value = 100.0;
        dial_sensor_scale.min_value = -100.0;
        dial_sensor_scale.set_value(sensor_scale_factor);

        if (dial_sensor_scale.is_initialized() == false) {
            throw std::runtime_error("Sensor Scaling Tuner not fully initialized!  Exiting.");
        }

        // Initialize P Gain Dial
        dial_P.name = "P Gain Dial";
        dial_P.dial_ = loaded_layout->findChild<QDial*>("dial_PGainTuner");
        if (dial_P.dial_) {
            connect(dial_P.dial_, &QSlider::valueChanged, this, &PIDTunerPlugin::knobPGainChanged);
        }
        dial_P.indicator_ = loaded_layout->findChild<QLabel*>("text_PGainValue");
        dial_P.label_max_value_ = loaded_layout->findChild<QLabel*>("text_dialPGain_Max");
        dial_P.label_min_value_ = loaded_layout->findChild<QLabel*>("text_dialPGain_Min");
        dial_P.max_value = 1.5;
        dial_P.min_value = -1.5;
        dial_P.set_value(K_P);

        if (dial_P.is_initialized() == false) {
            throw std::runtime_error("P Tuner not fully initialized!  Exiting.");
        }

        std::string setpoint_topic_name = "pidtuner_setpoint";
        setpoint_pub_ = nh_.advertise<std_msgs::Float32>(setpoint_topic_name, 1);
        latest_setpoint_ = 0.0;
        std::string sensor_topic_name = "pidtuner_sensor";
        sensor_sub_ = nh_.subscribe(sensor_topic_name, 100, &PIDTunerPlugin::sensor_Callback, this);

        std::string output_topic_name = "pidtuner_command";
        command_pub_ = nh_.advertise<std_msgs::Float32>(output_topic_name, 1);

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
        dial_set_point.update();
        dial_sensor_scale.update();
        dial_P.update();
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
    }
    double PIDTunerPlugin::scale_knob_value(int input, double max_, double min_) {
        // m = (y2-y1)/(x2-x1)
        double m = (max_ - min_) / 200.0;
        // y -y1 = m(x-x1) --> y = m(x-x1) + y1
        double output = m * ((double)input - 100.0) + max_;
        return output;
    }
}  // namespace robot_framework_ros

PLUGINLIB_EXPORT_CLASS(robot_framework_ros::PIDTunerPlugin, rqt_gui_cpp::Plugin)
