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
    void PIDTunerPlugin::setpoint_Callback(const std_msgs::Float32::ConstPtr& t_msg) { latest_setpoint_ = t_msg->data; }
    void PIDTunerPlugin::sensor_Callback(const std_msgs::Float32::ConstPtr& t_msg) { latest_sensor_ = t_msg->data; }
    void PIDTunerPlugin::knobPGainChanged(int value) { tuner_P.indicator_->setText(QString::number(value)); }
    void PIDTunerPlugin::knobSensorScaleChanged(int value) {
        tuner_sensor_scale.indicator_->setText(QString::number(value));
    }
    void PIDTunerPlugin::initPlugin(qt_gui_cpp::PluginContext& context) {
        widget_ = new QWidget();
        std::string package_path = ros::package::getPath("robot_framework_ros");
        std::string ui_file_path = package_path + "/tools/rqt_plugins/resource/pid_tuner_plugin.ui";

        QUiLoader loader;
        QFile file(QString::fromStdString(ui_file_path));
        if (!file.open(QFile::ReadOnly)) {
            ROS_ERROR("Failed to open UI file at: %s", ui_file_path.c_str());
            return;
        }

        // =========================================================================
        // FIX: Pass 'nullptr' instead of 'widget_'. Let the loader create it in isolation
        // =========================================================================
        QWidget* loaded_layout = loader.load(&file, nullptr);
        file.close();

        if (!loaded_layout) {
            ROS_ERROR("Failed to parse and load .ui design components!");
            return;
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

            // Formulate chart sheet layout
            chart_ = new QtCharts::QChart();
            chart_->addSeries(setpoint_series_);
            chart_->addSeries(sensor_series_);
            chart_->setTitle("Live System Response Tracking");

            // Configure axes
            axis_x_ = new QtCharts::QValueAxis();
            axis_x_->setTitleText("Time (s)");
            chart_->addAxis(axis_x_, Qt::AlignBottom);
            setpoint_series_->attachAxis(axis_x_);
            sensor_series_->attachAxis(axis_x_);

            axis_y_ = new QtCharts::QValueAxis();
            axis_y_->setTitleText("Value");
            axis_y_->setRange(-120.0, 120.0);  // Set expected robot limits
            chart_->addAxis(axis_y_, Qt::AlignLeft);
            setpoint_series_->attachAxis(axis_y_);
            sensor_series_->attachAxis(axis_y_);

            // Explicitly create the view wrapper frame in pure C++
            QtCharts::QChartView* chart_view = new QtCharts::QChartView(chart_, container);
            chart_view->setRenderHint(QPainter::Antialiasing);

            // Give the container widget a layout so the chart expands to fill its boundaries
            QVBoxLayout* container_layout = new QVBoxLayout(container);
            container_layout->setContentsMargins(0, 0, 0, 0);
            container_layout->addWidget(chart_view);
        } else {
            ROS_ERROR("Could not locate the 'chartContainer' layout placeholder box!");
        }

        // =========================================================================
        // NOTE: When looking for children, search 'loaded_layout' instead of 'widget_'
        // =========================================================================
        slider_ = loaded_layout->findChild<QSlider*>("horizontalSlider");
        if (slider_) {
            connect(slider_, &QSlider::valueChanged, this, &PIDTunerPlugin::onSliderMoved);
        }

        tuner_sensor_scale.dial_ = loaded_layout->findChild<QDial*>("dial_SensorScaleTuner");
        if (tuner_sensor_scale.dial_) {
            connect(tuner_sensor_scale.dial_, &QSlider::valueChanged, this, &PIDTunerPlugin::knobSensorScaleChanged);
        }
        tuner_sensor_scale.indicator_ = loaded_layout->findChild<QLabel*>("text_SensorScaleValue");

        if (tuner_sensor_scale.is_initialized() == false) {
            ROS_ERROR("Sensor Scaling Tuner not fully initialized!  Exiting.");
            return;
        }

        tuner_P.dial_ = loaded_layout->findChild<QDial*>("dial_PGainTuner");
        if (tuner_P.dial_) {
            connect(tuner_P.dial_, &QSlider::valueChanged, this, &PIDTunerPlugin::knobPGainChanged);
        }
        tuner_P.indicator_ = loaded_layout->findChild<QLabel*>("text_PGainValue");

        if (tuner_P.is_initialized() == false) {
            ROS_ERROR("P Tuner not fully initialized!  Exiting.");
            return;
        }

        std::string setpoint_topic_name = "/pidtuner_setpoint";
        setpoint_sub_ = nh_.subscribe(setpoint_topic_name, 100, &PIDTunerPlugin::setpoint_Callback, this);

        std::string sensor_topic_name = "/pidtuner_sensor";
        sensor_sub_ = nh_.subscribe(sensor_topic_name, 100, &PIDTunerPlugin::sensor_Callback, this);

        update_timer_ = new QTimer(this);
        connect(update_timer_, &QTimer::timeout, this, &PIDTunerPlugin::updateGraphLoop);
        update_timer_->start(33);
        // initialize start time for graphing
        start_time_ = ros::Time::now();
    }

    void PIDTunerPlugin::updateGraphLoop() {
        double elapsed_time = (ros::Time::now() - start_time_).toSec();

        // Add points to the Qt lines
        setpoint_series_->append(elapsed_time, latest_setpoint_);
        sensor_series_->append(elapsed_time, latest_sensor_);

        // Scroll the X axis window (shows last 10 seconds)
        if (elapsed_time > 10.0) {
            axis_x_->setRange(elapsed_time - 10.0, elapsed_time);
        } else {
            axis_x_->setRange(0, 10.0);
        }
    }

    void PIDTunerPlugin::shutdownPlugin() {
        // pub_.shutdown();
        // pub_slider_.shutdown();
        // update_timer_->stop();
    }

    void PIDTunerPlugin::onSliderMoved(int value) {
        // std_msgs::Int32 msg;
        // msg.data = value;
        // pub_slider_.publish(msg);

        ROS_INFO("Published new P value from GUI: %d", value);
    }

}  // namespace robot_framework_ros

PLUGINLIB_EXPORT_CLASS(robot_framework_ros::PIDTunerPlugin, rqt_gui_cpp::Plugin)
