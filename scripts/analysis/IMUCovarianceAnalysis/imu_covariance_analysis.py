

import sys,getopt,os
from optparse import OptionParser
import shutil
from pathlib import Path
import numpy as np
import yaml
import matplotlib.pyplot as plt
from mdutils.mdutils import MdUtils
from datetime import datetime
quiet_mode=False
parent_dir = str(Path(__file__).resolve().parent.parent)
if parent_dir not in sys.path:
    sys.path.insert(0, parent_dir)
from util.read_csv_ros_msg import read_csv_ros_msg
from util.helpers import matrix_to_latex_str, matrix_to_yaml_str
def compute_fft(signal,fs,title):
    n = len(signal)
    # 1. Run FFT (Subtract mean to remove any massive 0 Hz DC spike)
    fft_vals = np.fft.fft(signal - np.mean(signal))
    fft_freqs = np.fft.fftfreq(n, d=1 / fs)

    # 2. Extract positive frequencies only
    pos_mask = fft_freqs >= 0
    freqs = fft_freqs[pos_mask]
    magnitudes = np.abs(fft_vals[pos_mask])

    # 3. Create the figure and plot the spectrum
    # Using plt.subplots gives precise control over the figure object
    fig, ax = plt.subplots(figsize=(10, 5))

    ax.plot(freqs, magnitudes, color="blue", linewidth=1, alpha=0.85)
    ax.set_title(title)
    ax.set_xlabel("Frequency (Hz)")
    ax.set_ylabel("Linear Magnitude")
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(0, fs / 2)

    fig.tight_layout()

    # Return the figure object instead of calling plt.show()
    return fig

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
    
    with open(config_file, "r") as file:
        # safe_load prevents arbitrary code execution vulnerabilities
        config_data = yaml.safe_load(file) 

    generated_config_file = output_dir + "/" + config_data["info"]["name"] + ".yaml"
    with open(generated_config_file, "w") as file:
        yaml.dump(config_data, file)
    md_file = MdUtils(file_name=os.path.join(output_dir, config_data["info"]["name"]), title="IMU Covariance Analysis Report: "+ config_data["info"]["name"] ) 
  
    md_file.new_header(level=1, title="Overview")
    md_file.new_paragraph("Generated on: " + datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
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
        tempstr = "IMU Message Count: {}".format(len(data_series["imu"]))
        md_file.new_paragraph(tempstr)
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
        tempstr = "Orientation Roll Average: {:.8f} (rad) Standard Deviation: {:.8f}".format(np.mean(orientation_x), np.std(orientation_x))
        md_file.new_paragraph(tempstr)
        tempstr = "Orientation Pitch Average: {:.8f} (rad) Standard Deviation: {:.8f}".format(np.mean(orientation_y), np.std(orientation_y))
        md_file.new_paragraph(tempstr)
        tempstr = "Orientation Yaw Average: {:.8f} (rad) Standard Deviation: {:.8f}".format(np.mean(orientation_z), np.std(orientation_z))
        md_file.new_paragraph(tempstr)

        # Calculate covariance matrix for Orientation data
        orientation_covariance_matrix = np.cov(np.vstack((orientation_x, orientation_y, orientation_z)))
        tempstr = "Orientation Covariance Matrix:\n" + matrix_to_latex_str(orientation_covariance_matrix)
        md_file.new_paragraph(tempstr)
        
        with open(generated_config_file, "a") as file:
            yaml.safe_dump({"orientation_covariance_matrix": matrix_to_yaml_str(orientation_covariance_matrix)},file)

        md_file.new_paragraph("![](imu_orientation.png)")
        md_file.new_paragraph("![](imu_orientation_x_fft.png)")
        md_file.new_paragraph("![](imu_orientation_y_fft.png)")
        md_file.new_paragraph("![](imu_orientation_z_fft.png)")




        # Extract the Gyroscope data from the IMU messages
        gyro_x = [imu.angular_velocity.x for imu in data_series["imu"]]
        gyro_y = [imu.angular_velocity.y for imu in data_series["imu"]]
        gyro_z = [imu.angular_velocity.z for imu in data_series["imu"]]

        # Convert to numpy arrays for analysis
        gyro_x = np.array(gyro_x)
        gyro_y = np.array(gyro_y)
        gyro_z = np.array(gyro_z)

        

        md_file.new_header(level=2, title="Gyroscope Analysis")
        tempstr = "Gyroscope X Average: {:.8f} (rad/s) Standard Deviation: {:.8f}".format(np.mean(gyro_x), np.std(gyro_x))
        md_file.new_paragraph(tempstr)
        tempstr = "Gyroscope Y Average: {:.8f} (rad/s) Standard Deviation: {:.8f}".format(np.mean(gyro_y), np.std(gyro_y))
        md_file.new_paragraph(tempstr)
        tempstr = "Gyroscope Z Average: {:.8f} (rad/s) Standard Deviation: {:.8f}".format(np.mean(gyro_z), np.std(gyro_z))
        md_file.new_paragraph(tempstr)

        # Calculate covariance matrix for Gyroscope data
        gyro_covariance_matrix = np.cov(np.vstack((gyro_x, gyro_y, gyro_z)))
        tempstr = "Gyroscope Covariance Matrix:\n" + matrix_to_latex_str(gyro_covariance_matrix)
        md_file.new_paragraph(tempstr)
        with open(generated_config_file, "a") as file:
            yaml.safe_dump({"gyro_covariance_matrix": matrix_to_yaml_str(gyro_covariance_matrix)}, file)
        md_file.new_paragraph("![](imu_gyroscope.png)")
        md_file.new_paragraph("![](imu_gyroscope_x_fft.png)")
        md_file.new_paragraph("![](imu_gyroscope_y_fft.png)")
        md_file.new_paragraph("![](imu_gyroscope_z_fft.png)")

        

        # Extract linear acceleration data from IMU messages
        linear_acc_x = [imu.linear_acceleration.x for imu in data_series["imu"]]
        linear_acc_y = [imu.linear_acceleration.y for imu in data_series["imu"]]
        linear_acc_z = [imu.linear_acceleration.z for imu in data_series["imu"]]

        # Convert to numpy arrays for analysis
        linear_acc_x = np.array(linear_acc_x)
        linear_acc_y = np.array(linear_acc_y)
        linear_acc_z = np.array(linear_acc_z)

        md_file.new_header(level=2, title="Linear Acceleration Analysis")
        tempstr = "Linear Acceleration X Average: {:.8f} (m/s^2) Standard Deviation: {:.8f}".format(np.mean(linear_acc_x), np.std(linear_acc_x))
        md_file.new_paragraph(tempstr)
        tempstr = "Linear Acceleration Y Average: {:.8f} (m/s^2) Standard Deviation: {:.8f}".format(np.mean(linear_acc_y), np.std(linear_acc_y))
        md_file.new_paragraph(tempstr)
        tempstr = "Linear Acceleration Z Average: {:.8f} (m/s^2) Standard Deviation: {:.8f}".format(np.mean(linear_acc_z), np.std(linear_acc_z))
        md_file.new_paragraph(tempstr)
        # Calculate covariance matrix for the Linear Acceleration data
        linear_accel_covariance_matrix = np.cov(np.vstack((linear_acc_x, linear_acc_y, linear_acc_z)))

        tempstr = "Linear Acceleration Covariance Matrix:\n" + matrix_to_latex_str(linear_accel_covariance_matrix)
        md_file.new_paragraph(tempstr)
        with open(generated_config_file, "a") as file:
            yaml.safe_dump({"linear_accel_covariance_matrix": matrix_to_yaml_str(linear_accel_covariance_matrix)}, file)

        md_file.new_paragraph("![](imu_linear_acceleration.png)")
        md_file.new_paragraph("![](imu_linear_acceleration_x_fft.png)")
        md_file.new_paragraph("![](imu_linear_acceleration_y_fft.png)")
        md_file.new_paragraph("![](imu_linear_acceleration_z_fft.png)")

        fig_orientation = generate_figure(imu_time_stamps-imu_start_time, orientation_x, orientation_y, orientation_z,"IMU Orientation Data", "Orientation X (rad)", "Orientation Y (rad)", "Orientation Z (rad)")
        fig_orientation_x_fft =  compute_fft(orientation_x,imu_average_frequency,"Orientation X FFT")
        fig_orientation_y_fft =  compute_fft(orientation_y,imu_average_frequency,"Orientation Y FFT")
        fig_orientation_z_fft =  compute_fft(orientation_z,imu_average_frequency,"Orientation Z FFT")
        if show_plots:
            fig_orientation.show()
            fig_orientation_x_fft.show()
            fig_orientation_y_fft.show()
            fig_orientation_z_fft.show()

        fig_linear_acc = generate_figure(imu_time_stamps-imu_start_time, linear_acc_x, linear_acc_y, linear_acc_z,"IMU Linear Acceleration Data", "Acceleration X (m/s^2)", "Acceleration Y (m/s^2)", "Acceleration Z (m/s^2)")
        fig_linear_acc_x_fft =  compute_fft(linear_acc_x,imu_average_frequency,"Linear Acc X FFT")
        fig_linear_acc_y_fft =  compute_fft(linear_acc_y,imu_average_frequency,"Linear Acc Y FFT")
        fig_linear_acc_z_fft =  compute_fft(linear_acc_z,imu_average_frequency,"Linear Acc Z FFT")
        if show_plots:
            fig_linear_acc.show()
            fig_linear_acc_x_fft.show()
            fig_linear_acc_y_fft.show()
            fig_linear_acc_z_fft.show()

        fig_gyro = generate_figure(imu_time_stamps-imu_start_time, gyro_x, gyro_y, gyro_z,"IMU Gyroscope Data", "Gyroscope X (rad/s)", "Gyroscope Y (rad/s)", "Gyroscope Z (rad/s)")
        fig_gyro_x_fft = compute_fft(gyro_x,imu_average_frequency,"Gyro X FFT")
        fig_gyro_y_fft = compute_fft(gyro_x,imu_average_frequency,"Gyro Y FFT")
        fig_gyro_z_fft = compute_fft(gyro_x,imu_average_frequency,"Gyro Z FFT")
        if show_plots:
            fig_gyro.show()
            fig_gyro_x_fft.show()
            fig_gyro_y_fft.show()
            fig_gyro_z_fft.show()

        if save_output:
            fig_orientation.savefig(os.path.join(output_dir, "imu_orientation.png"),dpi=300, bbox_inches="tight")
            fig_orientation_x_fft.savefig(os.path.join(output_dir, "imu_orientation_x_fft.png"),dpi=300, bbox_inches="tight")
            fig_orientation_y_fft.savefig(os.path.join(output_dir, "imu_orientation_y_fft.png"),dpi=300, bbox_inches="tight")
            fig_orientation_z_fft.savefig(os.path.join(output_dir, "imu_orientation_z_fft.png"),dpi=300, bbox_inches="tight")
            fig_linear_acc.savefig(os.path.join(output_dir, "imu_linear_acceleration.png"),dpi=300, bbox_inches="tight")
            fig_linear_acc_x_fft.savefig(os.path.join(output_dir, "imu_linear_acceleration_x_fft.png"),dpi=300, bbox_inches="tight")
            fig_linear_acc_y_fft.savefig(os.path.join(output_dir, "imu_linear_acceleration_y_fft.png"),dpi=300, bbox_inches="tight")
            fig_linear_acc_z_fft.savefig(os.path.join(output_dir, "imu_linear_acceleration_z_fft.png"),dpi=300, bbox_inches="tight")
            fig_gyro.savefig(os.path.join(output_dir, "imu_gyroscope.png"),dpi=300, bbox_inches="tight")
            fig_gyro_x_fft.savefig(os.path.join(output_dir, "imu_gyroscope_x_fft.png"),dpi=300, bbox_inches="tight")
            fig_gyro_y_fft.savefig(os.path.join(output_dir, "imu_gyroscope_y_fft.png"),dpi=300, bbox_inches="tight")
            fig_gyro_z_fft.savefig(os.path.join(output_dir, "imu_gyroscope_z_fft.png"),dpi=300, bbox_inches="tight")
            print("[INFO] Saved plots to: " + output_dir)

    if(data_series["magnetic"] is None or len(data_series["magnetic"]) == 0):
        print("[WARN] No valid IMU Magnetic data found.")
    else:
        magnetic_time_stamps = [magnetic.header.stamp.to_sec() for magnetic in data_series["magnetic"]]
        magnetic_time_stamps = np.array(magnetic_time_stamps)
        magnetic_start_time = magnetic_time_stamps[0]
        magnetic_duration = magnetic_time_stamps[-1] - magnetic_start_time
        magnetic_average_frequency = len(magnetic_time_stamps) / magnetic_duration

        md_file.new_header(level=1, title="IMU Magnetometer Analysis")
        tempstr = "Magnetic Data Message Count: {}".format(len(data_series["magnetic"]))
        md_file.new_paragraph(tempstr)
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
        tempstr = "Magnetometer X Average: {:.8f} (T) Standard Deviation: {:.8f}".format(np.mean(magnetic_x), np.std(magnetic_x))
        md_file.new_paragraph(tempstr)
        tempstr = "Magnetometer Y Average: {:.8f} (T) Standard Deviation: {:.8f}".format(np.mean(magnetic_y), np.std(magnetic_y))
        md_file.new_paragraph(tempstr)
        tempstr = "Magnetometer Z Average: {:.8f} (T) Standard Deviation: {:.8f}".format(np.mean(magnetic_z), np.std(magnetic_z))
        md_file.new_paragraph(tempstr)

        # Calculate covariance matrix for Magnetometer data
        magnetic_covariance_matrix = np.cov(np.vstack((magnetic_x, magnetic_y, magnetic_z)))
        tempstr = "Magnetometer Covariance Matrix:\n" + matrix_to_latex_str(magnetic_covariance_matrix)
        md_file.new_paragraph(tempstr)
        with open(generated_config_file, "a") as file:
            yaml.safe_dump({"magnetic_covariance_matrix": matrix_to_yaml_str(magnetic_covariance_matrix)}, file) 
        md_file.new_paragraph("![](imu_magnetic_field.png)")
        md_file.new_paragraph("![](imu_magnetic_field_x_fft.png)")
        md_file.new_paragraph("![](imu_magnetic_field_y_fft.png)")
        md_file.new_paragraph("![](imu_magnetic_field_z_fft.png)")

        fig_magnetic = generate_figure(magnetic_time_stamps-magnetic_start_time, magnetic_x, magnetic_y, magnetic_z,"Magnetometer Data", "Magnetic Field X (T)", "Magnetic Field Y (T)", "Magnetic Field Z (T)")
        fig_magnetic_x_fft =  compute_fft(magnetic_x,imu_average_frequency,"Magnetometer X FFT")
        fig_magnetic_y_fft =  compute_fft(magnetic_y,imu_average_frequency,"Magnetometer Y FFT")
        fig_magnetic_z_fft =  compute_fft(magnetic_z,imu_average_frequency,"Magnetometer Z FFT")
        
        if show_plots:
            fig_magnetic.show()
            fig_magnetic_x_fft.show()
            fig_magnetic_y_fft.show()
            fig_magnetic_z_fft.show()

        if save_output:
            fig_magnetic.savefig(os.path.join(output_dir, "imu_magnetic_field.png"),dpi=300, bbox_inches="tight")
            fig_magnetic_x_fft.savefig(os.path.join(output_dir, "imu_magnetic_field_x_fft.png"),dpi=300, bbox_inches="tight")
            fig_magnetic_y_fft.savefig(os.path.join(output_dir, "imu_magnetic_field_y_fft.png"),dpi=300, bbox_inches="tight")
            fig_magnetic_z_fft.savefig(os.path.join(output_dir, "imu_magnetic_field_z_fft.png"),dpi=300, bbox_inches="tight")
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
