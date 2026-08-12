#include "robot_framework_ros/pid_tuner_plugin.h"

#include <pluginlib/class_list_macros.h>

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace robot_framework_ros {

    PIDTunerPlugin::PIDTunerPlugin() : rqt_gui_cpp::Plugin(), widget_(nullptr), slider_(nullptr) {
        setObjectName("PIDTunerPlugin");
    }

    void PIDTunerPlugin::initPlugin(qt_gui_cpp::PluginContext& context) {
        widget_ = new QWidget();
        widget_->setWindowTitle("Robot Framework PID Tuner Plugin");

        QVBoxLayout* layout = new QVBoxLayout(widget_);
        QPushButton* button = new QPushButton("Send Framework Command", widget_);
        layout->addWidget(button);
        slider_ = new QSlider(Qt::Horizontal, widget_);
        slider_->setMinimum(0);    // Minimum value bounds
        slider_->setMaximum(100);  // Maximum value bounds
        slider_->setValue(10);     // Initial startup position
        layout->addWidget(slider_);

        context.addWidget(widget_);
        pub_ = nh_.advertise<std_msgs::String>("gui_commands", 10);
        pub_slider_ = nh_.advertise<std_msgs::Int32>("pid_p_gain", 10);
        connect(slider_, &QSlider::valueChanged, this, &PIDTunerPlugin::onSliderValueChanged);
    }

    void PIDTunerPlugin::shutdownPlugin() {
        pub_.shutdown();
        pub_slider_.shutdown();
    }
    void PIDTunerPlugin::onSliderValueChanged(int value) {
        std_msgs::Int32 msg;
        msg.data = value;
        pub_slider_.publish(msg);

        ROS_INFO("Published new P value from GUI: %d", value);
    }

}  // namespace robot_framework_ros

PLUGINLIB_EXPORT_CLASS(robot_framework_ros::PIDTunerPlugin, rqt_gui_cpp::Plugin)
