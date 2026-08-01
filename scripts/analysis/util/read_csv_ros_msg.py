import rospy
import sys,getopt,os


from util.csv_to_ros_msg import parse_csv_ros_sensor_msgs_imu,parse_csv_ros_sensor_msgs_magnetic_field



def read_csv_ros_msg(csv_file,data_type):
    msg_series = []
    if not os.path.isfile(csv_file):
        print("[ERROR] File does not exist: " + csv_file)
        return msg_series
    with open(csv_file, 'r') as f:
        lines = f.readlines()
        first = True
        for line in lines:
            if first == True:
                first = False
                continue
            if data_type == "sensor_msgs/Imu":
                msg_data = parse_csv_ros_sensor_msgs_imu(line.strip())
                if msg_data is not None:
                    msg_series.append(msg_data)
            elif data_type == "sensor_msgs/MagneticField":
                msg_data = parse_csv_ros_sensor_msgs_magnetic_field(line.strip())
                if msg_data is not None:
                    msg_series.append(msg_data)
            else:
                print("[ERROR] Unsupported data type: " + data_type)
                return msg_series
        return msg_series