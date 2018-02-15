#include "control.h"

bool handleSensorUpdate(current_floor current, floor *floors, elev_motor_direction_t dir) {
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

bool doStartup(current_floor current, floor *floors, elev_motor_direction_t *dir, elev_motor_direction_t oldDir) {
	if (hasOrdersInDir(current, floors, oldDir)) {
		*dir = oldDir;
		return true;
	} else if (hasOrdersInDir(current, floors, oldDir * -1)) {
		*dir = oldDir * -1;
		return true;
	}
	return false;
}

void setMotorDir(elev_motor_direction_t dir) {
    elev_set_motor_direction(dir);
}
