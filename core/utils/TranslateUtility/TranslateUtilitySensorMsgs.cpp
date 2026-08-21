
#include <robot_framework_ros/utils/TranslateUtility.hpp>
namespace fast::rf_ros::utils {
    fast::rf::messages::SensorMsgs::JoyMsg TranslateUtility::convert(sensor_msgs::Joy msg) {
        fast::rf::messages::SensorMsgs::JoyMsg joy;
        joy.time_stamp = msg.header.stamp.toSec();
        joy.axes.assign(msg.axes.begin(), msg.axes.end());
        joy.buttons.assign(msg.buttons.begin(), msg.buttons.end());
        return joy;
    }

    sensor_msgs::Joy TranslateUtility::convert(fast::rf::messages::SensorMsgs::JoyMsg msg) {
        sensor_msgs::Joy joy;
        joy.header.stamp = ros::Time(msg.time_stamp);
        joy.axes.assign(msg.axes.begin(), msg.axes.end());
        joy.buttons.assign(msg.buttons.begin(), msg.buttons.end());
        return joy;
    }
    fast::rf::messages::SensorMsgs::ImuMsg TranslateUtility::convert(sensor_msgs::Imu data) {
        fast::rf::messages::SensorMsgs::ImuMsg msg;
        msg.time_stamp = data.header.stamp.toSec();
        msg.orientation = convert(data.orientation);
        msg.orientation = convert(data.orientation);
        msg.orientation_covariance = convert_covariance3D(data.orientation_covariance);
        msg.angular_velocity = convert(data.angular_velocity);
        msg.angular_velocity_covariance = convert_covariance3D(data.angular_velocity_covariance);
        msg.linear_acceleration = convert(data.linear_acceleration);
        msg.linear_acceleration_covariance = convert_covariance3D(data.linear_acceleration_covariance);
        return msg;
    }
    sensor_msgs::Imu TranslateUtility::convert(fast::rf::messages::SensorMsgs::ImuMsg data) {
        sensor_msgs::Imu msg;
        if (data.time_stamp < 0.0) {
            return msg;
        }
        msg.header.stamp = ros::Time(data.time_stamp);
        msg.orientation = convert(data.orientation);
        msg.orientation_covariance = convert_covariance3D(data.orientation_covariance);
        msg.angular_velocity = convert(data.angular_velocity);
        msg.angular_velocity_covariance = convert_covariance3D(data.angular_velocity_covariance);
        msg.linear_acceleration = convert(data.linear_acceleration);
        msg.linear_acceleration_covariance = convert_covariance3D(data.linear_acceleration_covariance);
        return msg;
    }
    fast::rf::messages::SensorMsgs::MagneticFieldMsg TranslateUtility::convert(sensor_msgs::MagneticField data) {
        fast::rf::messages::SensorMsgs::MagneticFieldMsg msg;
        msg.time_stamp = data.header.stamp.toSec();
        msg.magnetic_field = convert(data.magnetic_field);
        msg.magnetic_field_covariance = convert_covariance3D(data.magnetic_field_covariance);
        return msg;
    }
    sensor_msgs::MagneticField TranslateUtility::convert(fast::rf::messages::SensorMsgs::MagneticFieldMsg data) {
        sensor_msgs::MagneticField msg;
        if (data.time_stamp < 0.0) {
            return msg;
        }
        msg.header.stamp = ros::Time(data.time_stamp);
        msg.magnetic_field = convert(data.magnetic_field);
        msg.magnetic_field_covariance = convert_covariance3D(data.magnetic_field_covariance);
        return msg;
    }
}  // namespace fast::rf_ros::utils