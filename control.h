#include "floors.h"
#include "elev.h"
#include <stdbool.h>

bool handleSensorUpdate(current_floor current, floor *floors, elev_motor_direction_t dir);

bool doStartup(current_floor current, floor *floors, elev_motor_direction_t *dir, elev_motor_direction_t oldDir);

void setMotorDir(elev_motor_direction_t dir);
