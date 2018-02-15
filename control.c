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

bool hasOrdersInDir(current_floor current, floor *floors, elev_motor_direction_t dir) {
	if ((current == FLOOR_1 && dir == DIRN_DOWN) || (current == FLOOR_4 && dir == DIRN_UP)) {
		return false;
	}
	if (dir == DIRN_DOWN) {
		for (int i = current-1; i >= FLOOR_1; i--) {
			if (hasOrders(floors, i)) {
				return true;
			}
		}
	} else {
		for (int i = current+1; i <= FLOOR_4; i++) {
			if (hasOrders(floors, i)) {
				return true;
			}
		}
	}
	return false;
}

bool hasOrders(floor *floors, int index) {
	return floors[index].downOrder || floors[index].upOrder || floors[index].innerOrder;
}

void clearOrders(floor *floors, int index) {
	floors[index].downOrder = false;
	floors[index].upOrder = false;
	floors[index].innerOrder = false;
}
