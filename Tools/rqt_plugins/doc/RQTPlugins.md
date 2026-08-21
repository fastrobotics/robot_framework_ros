[Tools](../../doc/Tools.md)

- [RQT Plugins](#rqt-plugins)
  - [PID Tuning Plugin](#pid-tuning-plugin)
    - [ToDo](#todo)
    - [Overview](#overview)
    - [Pre-Requisites](#pre-requisites)
    - [Execution](#execution)
      - [Run a Transformer for the Sensor Input Topic:](#run-a-transformer-for-the-sensor-input-topic)
      - [Run a Transformer for the Set Point Topic:](#run-a-transformer-for-the-set-point-topic)
      - [Run the Tuner Plugin](#run-the-tuner-plugin)
# RQT Plugins
RQT Plugins are essentially ROS and QT plugins.
For development, open the QT Designer with:
```bash
designer
```

## PID Tuning Plugin

### ToDo
| ToDo                            |
| ------------------------------- |
| Stop from Seg Faulting on exit. |
| Fix `XmlRpcClient` write error  |
| Publish ROS configuration       |
| Form layout                     |
| Code Structure                  |
| Save PID Config                 |
| Load PID Config                 |

### Overview
The PID Tuning Plugin is a QT based GUI plugin that can be used to tune a PID Controller.

### Pre-Requisites
1. Determine what the Sensor Input topic is:
Example: `/robot/local_pose/twist/twist/angular/z`
2. Determine what the Setpoint Topic is:
Example: `/robot/cmd_desired_throttle/angular/z`
3. Determine what the Output Topic is:
Example: <blah>

### Execution
#### Run a Transformer for the Sensor Input Topic:
Run:
```bash
rosrun topic_tools transform <sensor input full topic> /pidtuner_sensor std_msgs/Float32 "m"
```
Example:
```bash
rosrun topic_tools transform /robot/local_pose/twist/twist/angular/z /pidtuner_sensor std_msgs/Float32 "m"
```

#### Run a Transformer for the Set Point Topic:
```bash
rosrun topic_tools transform <set point input full topic> /pidtuner_setpoint std_msgs/Float32 "m"
```
Example:
```bash
rosrun topic_tools transform /robot/cmd_desired_throttle/angular/z /pidtuner_setpoint std_msgs/Float32 "m"
```

#### Run the Tuner Plugin
```bash
rqt
```