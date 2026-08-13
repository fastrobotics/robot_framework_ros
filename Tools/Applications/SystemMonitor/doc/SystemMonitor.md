[Applications](../../doc/Applications.md)

# System Monitor

## Overview
The System Monitor Application provides a portable graphical interface to inspect the status of a Robot.

## Requirements
The System Monitor has the following requirements:
| Requirement                                                                                          |
| ---------------------------------------------------------------------------------------------------- |
| Can be run directly from command line                                                                |
| Can be run from a remote command line (ssh)                                                          |
| Is not required to be run on a robot.  Can be killed without any negative effect on robot operation. |
 
## Graphic Design
![](design/SystemMonitorGraphicDesign.png)

## Software Design
![](../../../../Legend.png)
### Class Diagram
![](puml/SystemMonitorClassDiagram.png)