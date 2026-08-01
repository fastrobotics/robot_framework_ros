import rospy
from sensor_msgs.msg import Imu

import csv
from io import StringIO

def parse_csv_ros_sensor_msgs_imu(csv_line):
    
    imu_msg = Imu()

    reader = csv.reader(StringIO(csv_line))
    tokens = next(reader)
    if len(tokens) != 42:
        print("[ERROR] Invalid CSV line for sensor_msgs/Imu: " + csv_line)
        return None
    seconds = int(tokens[2])
    nanoseconds = int(tokens[3])
    imu_msg.header.stamp.secs = seconds + nanoseconds / 1e9

    imu_msg.angular_velocity.x = float(tokens[18])
    imu_msg.angular_velocity.y = float(tokens[19])
    imu_msg.angular_velocity.z = float(tokens[20])

    
    imu_msg.linear_acceleration.x = float(tokens[30])
    imu_msg.linear_acceleration.y = float(tokens[31])
    imu_msg.linear_acceleration.z = float(tokens[32])

    

    return imu_msg 