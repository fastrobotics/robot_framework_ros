`@compare_tag Node-Document v0.1`
[Teleop Control Nodes](../../doc/Nodes-TeleopControl.md)
- [JoystickCalibration Node](#joystickcalibration-node)
- [Architecture](#architecture)
  - [Class Diagram](#class-diagram)
 - [Configuration](#configuration)
- [Usage Example](#usage-example)

# JoystickCalibration Node

# Architecture
![](../../../../../../../../Legend.png)

## Class Diagram
![](puml/JoystickCalibrationNodeClassDiagram.png)

# Configuration

# Usage Example
Typical use case of this node is to perform calibration manually.  This Node normally doesn't need to be run via a launch file.
```bash
rosrun robot_framework_ros nodeJoystickCalibration _x_deadband:=0.2 _y_deadband:=0.2 _throttle_deadband:=0.2 _output_file_path:=/home/david/git/component_database/Components/Electrical/JoysticsGamepads/ThrustmasterJoystick/ThrustmasterJoystick.yaml
```
