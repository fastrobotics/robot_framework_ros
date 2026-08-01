import rospy
from sensor_msgs.msg import Imu
from sensor_msgs.msg import MagneticField

import csv
from io import StringIO
from tf.transformations import euler_from_quaternion

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

    x = float(tokens[5])
    y = float(tokens[6])
    z = float(tokens[7])
    w = float(tokens[8])
    quaternion = (x, y, z, w)
    (roll, pitch, yaw) = euler_from_quaternion(quaternion)
    imu_msg.orientation.x = roll
    imu_msg.orientation.y = pitch
    imu_msg.orientation.z = yaw


    imu_msg.angular_velocity.x = float(tokens[18])
    imu_msg.angular_velocity.y = float(tokens[19])
    imu_msg.angular_velocity.z = float(tokens[20])

    
    imu_msg.linear_acceleration.x = float(tokens[30])
    imu_msg.linear_acceleration.y = float(tokens[31])
    imu_msg.linear_acceleration.z = float(tokens[32])

    

    return imu_msg 

def parse_csv_ros_sensor_msgs_magnetic_field(csv_line):
  
    magnetic_msg = MagneticField()

    reader = csv.reader(StringIO(csv_line))
    tokens = next(reader)
    if len(tokens) != 17:
        print("[ERROR] Invalid CSV line for sensor_msgs/MagneticField: " + csv_line)
        return None
    seconds = int(tokens[2])
    nanoseconds = int(tokens[3])
    magnetic_msg.header.stamp.secs = seconds + nanoseconds / 1e9

    magnetic_msg.magnetic_field.x = float(tokens[5])
    magnetic_msg.magnetic_field.y = float(tokens[6])
    magnetic_msg.magnetic_field.z = float(tokens[7])

    return magnetic_msg