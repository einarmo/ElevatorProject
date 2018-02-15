#include "floors.h"
#include "elev.h"
#include <stdbool.h>

bool handleSensorUpdate(current_floor current, floor *floors, elev_motor_direction_t dir);

bool doStartup(current_floor current, floor *floors, elev_motor_direction_t *dir, elev_motor_direction_t oldDir);

bool hasOrdersInDir(current_floor current, floor *floors, elev_motor_direction_t dir);

bool hasOrders(floor *floors, int index);

void clearOrders(floor *floors, int index);
