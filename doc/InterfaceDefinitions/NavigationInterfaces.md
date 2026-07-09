[Interface Definitions](InterfaceDefinition.md)

- [Navigation Interfaces](#navigation-interfaces)

# Navigation Interfaces

| Topic                  | Type                  | Interface Specification                                       | Publishers        | Subscribers         |
| ---------------------- | --------------------- | ------------------------------------------------------------- | ----------------- | ------------------- |
| `<robot>/joy`          | `sensor_msgs/Joy`     | Hardware specific, requires conversion in Teleop Control Node | joystick node     | Teleop Control Node |
| `<robot>/cmd_throttle` | `geometry_msgs/Twist` | Values are in Percentages, ranging from [-100.0,100.0]        |                   | DriveExecutor's     |
| `<robot>/left_drive`   | `std_msgs/Float64`    | PWM Values, Min and Max specified by config.                  | TankDriveExecutor |                     |
| `<robot>/right_drive`  | `std_msgs/Float64`    | PWM Values, Min and Max specified by config.                  | TankDriveExecutor |                     |