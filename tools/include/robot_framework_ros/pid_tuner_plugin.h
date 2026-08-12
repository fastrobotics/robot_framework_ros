#pragma once

#include <ros/ros.h>
#include <rqt_gui_cpp/plugin.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

#include <QSlider>
#include <QWidget>

namespace robot_framework_ros {

    class PIDTunerPlugin : public rqt_gui_cpp::Plugin {
        Q_OBJECT
       public:
        PIDTunerPlugin();
        virtual void initPlugin(qt_gui_cpp::PluginContext& context) override;
        virtual void shutdownPlugin() override;
       private slots:
        void onSliderValueChanged(int value);

       private:
        QWidget* widget_;
        QSlider* slider_;
        ros::NodeHandle nh_;
        ros::Publisher pub_;
        ros::Publisher pub_slider_;
    };

}  // namespace robot_framework_ros
