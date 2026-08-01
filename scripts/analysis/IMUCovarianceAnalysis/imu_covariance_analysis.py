

import sys,getopt,os
from optparse import OptionParser
import shutil
from pathlib import Path
import numpy as np
import yaml
import matplotlib.pyplot as plt
from mdutils.mdutils import MdUtils
quiet_mode=False
parent_dir = str(Path(__file__).resolve().parent.parent)
if parent_dir not in sys.path:
    sys.path.insert(0, parent_dir)
from util.read_csv_ros_msg import read_csv_ros_msg
from util.helpers import matrix_to_latex_str, matrix_to_yaml_str

def read_data(imu_csv_file, imu_magnetic_csv_file):
    imu_data = read_csv_ros_msg(imu_csv_file, "sensor_msgs/Imu")
    magnetic_data = read_csv_ros_msg(imu_magnetic_csv_file, "sensor_msgs/MagneticField")
    data_series = {
        "imu": imu_data,
        "magnetic": magnetic_data   
    }
    return data_series

def run_analysis(data_series, config_file="", save_output=False, show_plots=False,output_dir="~/tmp/output"):
    config_data = None
    generated_config_file = output_dir + "/config.yaml"
    with open(config_file, "r") as file:
        # safe_load prevents arbitrary code execution vulnerabilities
        config_data = yaml.safe_load(file) 
    with open(generated_config_file, "w") as file:
        yaml.dump(config_data, file)
    md_file = MdUtils(file_name=os.path.join(output_dir, config_data["info"]["name"]), title="IMU Covariance Analysis Report: "+config_data["info"]["name"] ) 
  
    md_file.new_header(level=1, title="Overview")
    md_file.new_paragraph("This report provides an analysis of the IMU data, including covariance matrices for orientation, gyroscope, linear acceleration, and magnetometer data. The analysis is based on the provided CSV files containing IMU and magnetic field data.")    

    md_file.new_header(level=2, title="IMU Parameters")
    for param, value in config_data["info"].items():
        md_file.new_paragraph(f"- {param}: `{value}`")

    
    if(data_series["imu"] is None or len(data_series["imu"]) == 0):
        print("[WARN] No valid IMU data found.")
    else:

        
        imu_time_stamps = [imu.header.stamp.to_sec() for imu in data_series["imu"]]
        imu_time_stamps = np.array(imu_time_stamps)
        imu_start_time = imu_time_stamps[0]
        imu_duration = imu_time_stamps[-1] - imu_start_time
        imu_average_frequency = len(imu_time_stamps) / imu_duration

        md_file.new_header(level=1, title="IMU Analysis")
        tempstr = "IMU Data Duration: {:.2f} seconds".format(imu_duration)
        md_file.new_paragraph(tempstr)
        tempstr = "IMU Data Average Frequency: {:.2f} Hz".format(imu_average_frequency)
        md_file.new_paragraph(tempstr)

        # Extract the Orientation data from the IMU messages
        orientation_x = [imu.orientation.x for imu in data_series["imu"]]
        orientation_y = [imu.orientation.y for imu in data_series["imu"]]
        orientation_z = [imu.orientation.z for imu in data_series["imu"]]

        # Convert to numpy arrays for analysis
        orientation_x = np.array(orientation_x)
        orientation_y = np.array(orientation_y)
        orientation_z = np.array(orientation_z)

        md_file.new_header(level=2, title="Orientation Analysis")

        # Calculate covariance matrix for Orientation data
        orientation_covariance_matrix = np.cov(np.vstack((orientation_x, orientation_y, orientation_z)))
        tempstr = "Orientation Covariance Matrix:\n" + matrix_to_latex_str(orientation_covariance_matrix)
        md_file.new_paragraph(tempstr)
        with open(generated_config_file, "a") as file:
            yaml.safe_dump({"orientation_covariance_matrix": matrix_to_yaml_str(orientation_covariance_matrix)}, file)

        md_file.new_paragraph("![](imu_orientation.png)")


        # Extract the Gyroscope data from the IMU messages
        gyro_x = [imu.angular_velocity.x for imu in data_series["imu"]]
        gyro_y = [imu.angular_velocity.y for imu in data_series["imu"]]
        gyro_z = [imu.angular_velocity.z for imu in data_series["imu"]]

        # Convert to numpy arrays for analysis
        gyro_x = np.array(gyro_x)
        gyro_y = np.array(gyro_y)
        gyro_z = np.array(gyro_z)

        md_file.new_header(level=2, title="Gyroscope Analysis")
        # Calculate covariance matrix for Gyroscope data
        gyro_covariance_matrix = np.cov(np.vstack((gyro_x, gyro_y, gyro_z)))
        tempstr = "Gyroscope Covariance Matrix:\n" + matrix_to_latex_str(gyro_covariance_matrix)
        md_file.new_paragraph(tempstr)
        with open(generated_config_file, "a") as file:
            yaml.safe_dump({"gyro_covariance_matrix": matrix_to_yaml_str(gyro_covariance_matrix)}, file)
        md_file.new_paragraph("![](imu_gyroscope.png)")

        # Extract linear acceleration data from IMU messages
        linear_acc_x = [imu.linear_acceleration.x for imu in data_series["imu"]]
        linear_acc_y = [imu.linear_acceleration.y for imu in data_series["imu"]]
        linear_acc_z = [imu.linear_acceleration.z for imu in data_series["imu"]]

        # Convert to numpy arrays for analysis
        linear_acc_x = np.array(linear_acc_x)
        linear_acc_y = np.array(linear_acc_y)
        linear_acc_z = np.array(linear_acc_z)

        md_file.new_header(level=2, title="Linear Acceleration Analysis")
        # Calculate covariance matrix for the Linear Acceleration data
        linear_accel_covariance_matrix = np.cov(np.vstack((linear_acc_x, linear_acc_y, linear_acc_z)))

        tempstr = "Linear Acceleration Covariance Matrix:\n" + matrix_to_latex_str(linear_accel_covariance_matrix)
        md_file.new_paragraph(tempstr)
        with open(generated_config_file, "a") as file:
            yaml.safe_dump({"linear_accel_covariance_matrix": matrix_to_yaml_str(linear_accel_covariance_matrix)}, file)

        md_file.new_paragraph("![](imu_linear_acceleration.png)")

        fig_orientation = generate_figure(imu_time_stamps-imu_start_time, orientation_x, orientation_y, orientation_z,"IMU Orientation Data", "Orientation X (rad)", "Orientation Y (rad)", "Orientation Z (rad)")
        if show_plots:
            fig_orientation.show()

        fig_linear_acc = generate_figure(imu_time_stamps-imu_start_time, linear_acc_x, linear_acc_y, linear_acc_z,"IMU Linear Acceleration Data", "Acceleration X (m/s^2)", "Acceleration Y (m/s^2)", "Acceleration Z (m/s^2)")
        if show_plots:
            fig_linear_acc.show()

        fig_gyro = generate_figure(imu_time_stamps-imu_start_time, gyro_x, gyro_y, gyro_z,"IMU Gyroscope Data", "Gyroscope X (rad/s)", "Gyroscope Y (rad/s)", "Gyroscope Z (rad/s)")
        if show_plots:
            fig_gyro.show()

        if save_output:
            fig_orientation.savefig(os.path.join(output_dir, "imu_orientation.png"),dpi=300, bbox_inches="tight")
            fig_linear_acc.savefig(os.path.join(output_dir, "imu_linear_acceleration.png"),dpi=300, bbox_inches="tight")
            fig_gyro.savefig(os.path.join(output_dir, "imu_gyroscope.png"),dpi=300, bbox_inches="tight")
            print("[INFO] Saved plots to: " + output_dir)

    if(data_series["magnetic"] is None or len(data_series["magnetic"]) == 0):
        print("[WARN] No valid IMU Magnetic data found.")
    else:
        md_file.new_header(level=1, title="IMU Magnetometer Analysis")
        magnetic_time_stamps = [magnetic.header.stamp.to_sec() for magnetic in data_series["magnetic"]]
        magnetic_time_stamps = np.array(magnetic_time_stamps)
        magnetic_start_time = magnetic_time_stamps[0]
        magnetic_duration = magnetic_time_stamps[-1] - magnetic_start_time
        magnetic_average_frequency = len(magnetic_time_stamps) / magnetic_duration
        tempstr = "Magnetic Data Duration: {:.2f} seconds".format(magnetic_duration)
        md_file.new_paragraph(tempstr)
        
        tempstr = "Magnetic Data Average Frequency: {:.2f} Hz".format(magnetic_average_frequency)
        md_file.new_paragraph(tempstr)

        # Extract the Magnetometer data from the Magnetic Field messages
        magnetic_x = [magnetic.magnetic_field.x for magnetic in data_series["magnetic"]]
        magnetic_y = [magnetic.magnetic_field.y for magnetic in data_series["magnetic"]]
        magnetic_z = [magnetic.magnetic_field.z for magnetic in data_series["magnetic"]]

        # Convert to numpy arrays for analysis
        magnetic_x = np.array(magnetic_x)
        magnetic_y = np.array(magnetic_y)
        magnetic_z = np.array(magnetic_z)

        md_file.new_header(level=2, title="Magnetometer Analysis")
        # Calculate covariance matrix for Magnetometer data
        magnetic_covariance_matrix = np.cov(np.vstack((magnetic_x, magnetic_y, magnetic_z)))
        tempstr = "Magnetometer Covariance Matrix:\n" + matrix_to_latex_str(magnetic_covariance_matrix)
        md_file.new_paragraph(tempstr)
        with open(generated_config_file, "a") as file:
            yaml.safe_dump({"magnetic_covariance_matrix": matrix_to_yaml_str(magnetic_covariance_matrix)}, file) 
        md_file.new_paragraph("![](imu_magnetic_field.png)")

        fig_magnetic = generate_figure(magnetic_time_stamps-magnetic_start_time, magnetic_x, magnetic_y, magnetic_z,"Magnetometer Data", "Magnetic Field X (T)", "Magnetic Field Y (T)", "Magnetic Field Z (T)")
        if show_plots:
            fig_magnetic.show()

        if save_output:
            fig_magnetic.savefig(os.path.join(output_dir, "imu_magnetic_field.png"),dpi=300, bbox_inches="tight")
            print("[INFO] Saved plots to: " + output_dir)
    if save_output:
        md_file.new_table_of_contents(table_title="Table of Contents", depth=2)
        md_file.create_md_file()
    if show_plots:
        input("Press Enter to close the plots and exit...")

def generate_figure(time_stamps,ch1_data,ch2_data,ch3_data,fig_title,ch1_label,ch2_label,ch3_label):
    import matplotlib.pyplot as plt
    fig,(ax1,ax2,ax3) = plt.subplots(3, 1, figsize=(12, 6),num=fig_title)
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
    parser.add_option("-p","--plot-show",action="store_true",dest="show_plots",default=False,help="Show the IMU Plots")
    parser.add_option("-s","--save",action="store_true",dest="save_output",default=True,help="Save the output to files")
    parser.add_option("-c","--config-file",dest="config_file",default="",help="Configuration file for IMU parameters")
    parser.add_option("-o","--output-dir",dest="output_dir",default="~/tmp/output",help="Output directory for saving data")
    parser.add_option("-i","--imu-csv-file",dest="imu_csv_file",default="",help="IMU CSV File")
    parser.add_option("-m","--imu-magnetic-csv-file",dest="imu_magnetic_csv_file",default="",help="IMU Magnetic CSV File")
    (opts,args) = parser.parse_args()
    if not opts.imu_csv_file and not opts.imu_magnetic_csv_file:
        print("[ERROR] Please provide at least one CSV file for analysis.")
        sys.exit(1) 
    if os.path.exists(opts.output_dir):
        shutil.rmtree(opts.output_dir)
    os.makedirs(opts.output_dir)
    data_series = read_data(opts.imu_csv_file,opts.imu_magnetic_csv_file)
    run_analysis(data_series,opts.config_file,opts.save_output,opts.show_plots,opts.output_dir)


if __name__ == "__main__":
    main()
