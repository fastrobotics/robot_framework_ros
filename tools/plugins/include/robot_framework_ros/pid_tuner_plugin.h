#pragma once

#include <ros/ros.h>
#include <rqt_gui_cpp/plugin.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

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
        Q_OBJECT
       public:
        PIDTunerPlugin();
        virtual void initPlugin(qt_gui_cpp::PluginContext& context) override;
        virtual void shutdownPlugin() override;
        void setpoint_Callback(const std_msgs::Float32::ConstPtr& t_msg);
        void sensor_Callback(const std_msgs::Float32::ConstPtr& t_msg);
       private slots:
        void onSliderMoved(int value);
        void updateGraphLoop();

       private:
        QWidget* widget_{nullptr};
        QSlider* slider_{nullptr};
        QtCharts::QLineSeries* setpoint_series_{nullptr};
        QtCharts::QLineSeries* sensor_series_{nullptr};
        QtCharts::QChart* chart_{nullptr};
        QtCharts::QChartView* chart_view_{nullptr};
        QtCharts::QValueAxis* axis_x_{nullptr};
        QtCharts::QValueAxis* axis_y_{nullptr};

        QTimer* update_timer_{nullptr};
        ros::Time start_time_;

        ros::NodeHandle nh_;
        ros::Publisher pub_;
        ros::Publisher pub_slider_;
        ros::Subscriber setpoint_sub_;
        ros::Subscriber sensor_sub_;
        double latest_setpoint_{0.0};
        double latest_sensor_{0.0};
    };

}  // namespace robot_framework_ros
