[README](../../../README.md)

- [Analysis Guide](#analysis-guide)
- [General Workflow](#general-workflow)
  - [Preq-Requisites](#preq-requisites)
  - [Task - Extract Bags](#task---extract-bags)
- [Analysis Proccesses](#analysis-proccesses)
- [IMU Covariance Analysis](#imu-covariance-analysis)
  - [Setup](#setup)
  - [Execution](#execution)
- [Helpful Commands](#helpful-commands)
  - [UDEV Device Lookup](#udev-device-lookup)

# Analysis Guide

# General Workflow
Typical Analysis includes the following steps:
## Preq-Requisites
1. Perform some data collection on the robot and/or archive existing logs.
2. Extract ROS Bag's to CSV's: [Extract Bags](#task---extract-bags)
3. Pick an [Analysis Process](#analysis-proccesses)


## Task - Extract Bags
Use this script:
```bash
python scripts/extract_bag_to_csv.py --bag-file <Bag File>
```
Which will convert all messages in a bag file to csv files in a folder with the input bag file

# Analysis Proccesses
The following Analysis Processes are supported:
| Process                                             | Description                                                        |
| --------------------------------------------------- | ------------------------------------------------------------------ |
| [IMU Covariance Analysis](#imu-covariance-analysis) | A tool that is used to analyze IMU data for covariance properties. |


# IMU Covariance Analysis
## Setup
1. Determine the following attributes of the analysis:
- Which IMU CSV channel(s) should be read.  Typically for this analysis, at least one of the following are required, but both are desirable:
    - a CSV File "IMUDataChannel" that supports `IMUMsg` Data (such as ROSv1 sensor_msgs/Imu)
    - a CSV File "IMUMagneticDataChannel" that supports `MagneticFieldMsg` Data (such as ROSv1 sensor_msgs/MagneticField)
  
1. Construct a configuration yaml file:
```yaml
info:
  name: <Name of IMU>
  device_name: <Linux File Path>
  description: <Human description of device>
  manufacturer: <Manufacturer>
  full_serial_number: <Full Serial Number, use `udev` lookup below>
```
## Execution
Run:
```bash
python scripts/analysis/IMUCovarianceAnalysis/imu_covariance_analysis.py --config-file <path to IMU config yaml> --output-dir <Ouput Directory for artifacts> --imu-csv-file <Path to IMUDataChannel.csv> --imu-magnetic-csv-file <Path to IMUMagneticDataChannel.csv> 
```
  

# Helpful Commands
## UDEV Device Lookup
```bash
udevadm info -a -n <Linux Device Path>
```