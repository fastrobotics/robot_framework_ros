# PID Tuning Plugin
## Pre-Requisites
1. Determine what the Sensor Input topic is:
Example: `/robot/local_pose/twist/twist/angular/z`
2. Determine what the Setpoint Topic is:
Example: `/robot/cmd_desired_throttle/angular/z`
3. Determine what the Output Topic is:
Example: <blah>

## Execution
### Run a Transformer for the Sensor Input Topic:
Run:
```bash
rosrun topic_tools transform <sensor input full topic> /pidtuner_sensor std_msgs/Float32 "m"
```
Example:
```bash
rosrun topic_tools transform /robot/local_pose/twist/twist/angular/z /pidtuner_sensor std_msgs/Float32 "m"
```

### Run a Transformer for the Set Point Topic:
```bash
rosrun topic_tools transform <set point input full topic> /pidtuner_setpoint std_msgs/Float32 "m"
```
Example:
```bash
rosrun topic_tools transform /robot/cmd_desired_throttle/angular/z /pidtuner_setpoint std_msgs/Float32 "m"
```

### Run the Tuner Plugin
```bash
rqt
```