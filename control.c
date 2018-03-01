#include "control.h"

bool handleSensorUpdate(floor_num current, floor *floors, elev_motor_direction_t dir) {
    bool upOrder = floors[current].upOrder;
    bool downOrder = floors[current].downOrder;
    bool innerOrder = floors[current].innerOrder;
    if (current == FLOOR_4
        || current == FLOOR_1
        || innerOrder
        || (downOrder && dir == DIRN_DOWN)
        || (downOrder && dir == DIRN_UP && !hasOrdersInDir(current, floors, dir))
        || (upOrder && dir == DIRN_UP)
        || (upOrder && dir == DIRN_DOWN && !hasOrdersInDir(current, floors, dir))) {
        clearOrders(floors, current);
        return true;
    }
    return false;
}

bool doStartup(floor_num *current, floor *floors, elev_motor_direction_t *dir, elev_motor_direction_t oldDir, bool fromRest) {
    if (hasOrdersInDir(*current, floors, oldDir)) {
        *dir = oldDir;
        return true;
    }
    // If function is called from rest and elevator is between floors then the stop button
    // was pressed, and we have to check if the elevator has orders at the floor it came from.
    if (fromRest && elev_get_floor_sensor_signal() == -1 && hasOrders(floors, *current)) {
        *dir = oldDir * -1;
        *current += oldDir;
        return true;
    }
    if (hasOrdersInDir(*current, floors, oldDir * -1)) {
        *dir = oldDir * -1;
        return true;
    }
    return false;
}

void setMotorDir(elev_motor_direction_t dir) {
    elev_set_motor_direction(dir);
}
