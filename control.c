#include "control.h"

bool handleSensorUpdate(current_floor current, floor *floors, elev_motor_direction_t dir) {
	bool upOrder = floors[current].upOrder;
	bool downOrder = floors[current].downOrder;
	bool innerOrder = floors[current].innerOrder;
	floors[current].innerOrder = false;
	if (current == FLOOR_4 || current == FLOOR_1) {
		floors[current].downOrder = false;
		floors[current].upOrder = false;
		return true;
	}
	if (innerOrder
	    || (downOrder && dir == DIRN_DOWN)
	    || (downOrder && dir == DIRN_UP && checkOppositeStop(current, floors, dir))
	    || (upOrder && dir == DIRN_UP)
	    || (upOrder && dir == DIRN_DOWN && checkOppositeStop(current, floors, dir))) {
		floors[current].downOrder = false;
		floors[current].upOrder = false;
		return true;
	}
	return false;
}

bool doStartup(current_floor current, floor *floors, elev_motor_direction_t *dir, elev_motor_direction_t oldDir) {
	if (checkOppositeStop(current, floors, oldDir)) {
		*dir = oldDir;
		return true;
	} else if (checkOppositeStop(current, floors, oldDir * -1)) {
		*dir = oldDir * -1;
		return true;
	}
	return false;
}

bool checkOppositeStop(current_floor current, floor *floors, elev_motor_direction_t dir) {
	if (current == FLOOR_1 || current == FLOOR_4) {
		return true;
	}
	if (dir == DIRN_DOWN) {
		for (int i = current-1; i >= FLOOR_1; i--) {
			if (floors[i].downOrder || floors[i].upOrder || floors[i].innerOrder) {
				return false;
			}
		}
	} else {
		for (int i = current+1; i <= FLOOR_4; i++) {
			if (floors[i].downOrder || floors[i].upOrder || floors[i].innerOrder) {
				return false;
			}
		}
	}
	return true;
}
