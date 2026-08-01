[README](../../../README.md)

- [Analysis Guide](#analysis-guide)
- [General Workflow](#general-workflow)
  - [Preq-Requisites](#preq-requisites)
  - [Task - Extract Bags](#task---extract-bags)
- [Analysis Proccesses](#analysis-proccesses)
- [IMU Covariance Analysis](#imu-covariance-analysis)
  - [Setup](#setup)
  - [Execution](#execution)

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
Determine the following attributes of the analysis:
- IMU Vendor
- IMU Device Name
- Which IMU CSV channel(s) should be read.  Typically for this analysis, at least one of the following are required, but both are desirable:
    - a CSV File "IMUDataChannel" that supports `IMUMsg` Data (such as ROSv1 sensor_msgs/Imu)
    - a CSV File "IMUMagneticDataChannel" that supports `MagneticFieldMsg` Data (such as ROSv1 sensor_msgs/MagneticField)
  

## Execution
Run:
```bash
python scripts/analysis/IMUCovarianceAnalysis/imu_covariance_analysis.py --imu-csv-file <Path to IMUDataChannel.csv> 
```
  

