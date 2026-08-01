

import sys,getopt,os
from optparse import OptionParser
import shutil
from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt

parent_dir = str(Path(__file__).resolve().parent.parent)
if parent_dir not in sys.path:
    sys.path.insert(0, parent_dir)
from util.read_csv_ros_msg import read_csv_ros_msg

def read_data(imu_csv_file):
    imu_data = read_csv_ros_msg(imu_csv_file, "sensor_msgs/Imu")
    return imu_data

def run_analysis(imu_series, plotting_enabled=False):
    

    time_stamps = [imu.header.stamp.to_sec() for imu in imu_series]
    time_stamps = np.array(time_stamps)
    start_time = time_stamps[0]
    duration = time_stamps[-1] - start_time
    average_frequency = len(time_stamps) / duration
    print("IMU Data Duration: {:.2f} seconds".format(duration))
    print("IMU Data Average Frequency: {:.2f} Hz".format(average_frequency))

    # Extract the Gyroscope data from the IMU messages
    gyro_x = [imu.angular_velocity.x for imu in imu_series]
    gyro_y = [imu.angular_velocity.y for imu in imu_series]
    gyro_z = [imu.angular_velocity.z for imu in imu_series]

    # Convert to numpy arrays for analysis
    gyro_x = np.array(gyro_x)
    gyro_y = np.array(gyro_y)
    gyro_z = np.array(gyro_z)

    # Calculate covariance matrix for Gyroscope data
    gyro_covariance_matrix = np.cov(np.vstack((gyro_x, gyro_y, gyro_z)))
    print("\n\nGyroscope Covariance Matrix:")
    print(gyro_covariance_matrix)


    # Extract linear acceleration data from IMU messages
    linear_acc_x = [imu.linear_acceleration.x for imu in imu_series]
    linear_acc_y = [imu.linear_acceleration.y for imu in imu_series]
    linear_acc_z = [imu.linear_acceleration.z for imu in imu_series]

    # Convert to numpy arrays for analysis
    linear_acc_x = np.array(linear_acc_x)
    linear_acc_y = np.array(linear_acc_y)
    linear_acc_z = np.array(linear_acc_z)

    # Calculate covariance matrix for the Linear Acceleration data
    linear_accel_covariance_matrix = np.cov(np.vstack((linear_acc_x, linear_acc_y, linear_acc_z)))

    print("\n\nLinear Acceleration Covariance Matrix:")
    print(linear_accel_covariance_matrix)


  


    

    if plotting_enabled:
        fig_linear_acc = generate_figure(time_stamps-start_time, linear_acc_x, linear_acc_y, linear_acc_z,"IMU Linear Acceleration Data", "Acceleration X (m/s^2)", "Acceleration Y (m/s^2)", "Acceleration Z (m/s^2)")
        fig_linear_acc.show()

        fig_gyro = generate_figure(time_stamps-start_time, gyro_x, gyro_y, gyro_z,"IMU Gyroscope Data", "Gyroscope X (rad/s)", "Gyroscope Y (rad/s)", "Gyroscope Z (rad/s)")
        fig_gyro.show()

        input("Press Enter to close the plots and exit...")

def generate_figure(time_stamps,ch1_data,ch2_data,ch3_data,fig_title,ch1_label,ch2_label,ch3_label):
    import matplotlib.pyplot as plt
    fig,(ax1,ax2,ax3) = plt.subplots(3, 1, figsize=(12, 6))
    fig.suptitle(fig_title)
    ax1.plot(time_stamps, ch1_data, label=ch1_label)
    ax1.set_ylabel(ch1_label)
    ax1.legend()
    
    ax2.plot(time_stamps, ch2_data, label=ch2_label)
    ax2.set_ylabel(ch2_label)
    ax2.legend()
    
    ax3.plot(time_stamps, ch3_data, label=ch3_label)
    ax3.set_ylabel(ch3_label)
    ax3.legend()
    
    fig.tight_layout()
    return fig 
def main():
    parser = OptionParser()
    parser.add_option("-p","--plot",action="store_true",dest="plotting_enabled",default=True,help="Plot the IMU data")
    parser.add_option("-i","--imu-csv-file",dest="imu_csv_file",default="",help="IMU CSV File")
    (opts,args) = parser.parse_args()
    imu_series = read_data(opts.imu_csv_file)
    run_analysis(imu_series,opts.plotting_enabled)


if __name__ == "__main__":
    main()
