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
        counter = 0
        for line in lines:
            if first == True:
                first = False
                continue
            if counter % 1000 == 0:
                remaining_amount_perc = ((len(lines)-1 - counter)/(len(lines)-1)) * 100

                print(f"[INFO] Lines Processed: [{counter}/{len(lines)-1}] Remaining: {remaining_amount_perc:.2f}% for CSV file: {csv_file}")

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
            counter = counter + 1
        return msg_series