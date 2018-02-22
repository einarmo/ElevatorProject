#include "floors.h"
#include <stdbool.h>

// Handle sensor update, assumes that the sensor status was changed and returns true if
// the elevator should stop.
bool handleSensorUpdate(floor_num current, floor *floors, elev_motor_direction_t dir);

// Returns true if the elevator is to start moving, updates dir to the correct direction
bool doStartup(floor_num *current, floor *floors, elev_motor_direction_t *dir, elev_motor_direction_t oldDir, bool fromRest);

// Sets motor direction on the elevator.
void setMotorDir(elev_motor_direction_t dir);
