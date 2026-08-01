import rospy
import sys,getopt,os


# Message Definitions
from sensor_msgs.msg import Imu

from util.csv_to_ros_msg import parse_csv_ros_sensor_msgs_imu



def read_csv_ros_msg(csv_file,data_type):
    msg_series = []
    if not os.path.isfile(csv_file):
        print("[ERROR] File does not exist: " + csv_file)
        return msg_data
    with open(csv_file, 'r') as f:
        lines = f.readlines()
        first = True
        for line in lines:
            if first == True:
                first = False
                continue
            if data_type == "sensor_msgs/Imu":
                msg_data = parse_csv_ros_sensor_msgs_imu(line.strip())
                msg_series.append(msg_data)
            else:
                print("[ERROR] Unsupported data type: " + data_type)
                return msg_series
        return msg_series