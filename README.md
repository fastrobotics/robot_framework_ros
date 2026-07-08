# FAST Robotics - Robot Framework: ROS v1 Middleware

- [FAST Robotics - Robot Framework: ROS v1 Middleware](#fast-robotics---robot-framework-ros-v1-middleware)
- [Architecture](#architecture)
- [Systems](#systems)
- [Features](#features)
- [ToDo](#todo)
  - [This PR](#this-pr)
- [Setup](#setup)
- [Documentation](#documentation)
  - [Interface Documentation](#interface-documentation)

# Architecture
![](Legend.png)

# Systems
| Status | System                                                                     |
| ------ | -------------------------------------------------------------------------- |
| DRAFT  | [Navigation System](Systems/Navigation/doc/System-Navigation.md)           |
| DRAFT  | [User Interface System](Systems/UserInterface/doc/System-UserInterface.md) |


# Features
| Status | Feature                                             |
| ------ | --------------------------------------------------- |
| DRAFT  | [Core](include/robot_framework_ros/doc/Core.md)     |
| DRAFT  | [Example Node](core/ExampleNode/doc/ExampleNode.md) |


# ToDo
| Item                                          |
| --------------------------------------------- |
| Change message definition namespace           |
| Add code coverage for Example Node            |
| Add guide for creating new node from BaseNode |
| Doxygen hosted documentation                  |
| Doxygen for messages?                         |


## This PR
| Item                             |
| -------------------------------- |
| Add topic names to configuration |


# Setup

Pre-Requisites:

- Ubuntu system running 20.04 LTS

1. Clone this repo using:
```bash
git clone --recurse-submodules https://github.com/fastrobotics/robot_framework_ros.git
git submodule update --remote
```
2. Run the following:

```bash
cd <repo>
./scripts/setup_ide.sh
./scripts/setup_robot.sh
```

# Documentation
## Interface Documentation
[Interface Documentation](doc/InterfaceDefinitions/InterfaceDefinition.md)
