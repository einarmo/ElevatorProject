#include "floors.h"

void updateFloorStatus(floor *floors) {
	for (int i = FLOOR_2; i <= FLOOR_4; i++) {
		floors[i].downOrder = floors[i].downOrder || elev_get_button_signal(BUTTON_CALL_DOWN, i);
	}
	for (int i = FLOOR_1; i <= FLOOR_3; i++) {
		floors[i].upOrder = floors[i].upOrder || elev_get_button_signal(BUTTON_CALL_UP, i);
	}
	for (int i = FLOOR_1; i <= FLOOR_4; i++) {
		floors[i].innerOrder = floors[i].innerOrder || elev_get_button_signal(BUTTON_COMMAND, i);
	}
}

bool updateCurrentFloor(current_floor *current) {
	int get = elev_get_floor_sensor_signal();
	if (get != -1) {
		if (*current != get) {
			*current = get;
			return true;
		}
		return false;
	}
	return false;
}

void updateFloorLight(current_floor current) {
	if (current != -1) {
		elev_set_floor_indicator(current);
	}
}

void updateLights(floor *floors) {
	for (int i = FLOOR_2; i <= FLOOR_4; i++) {
		elev_set_button_lamp(BUTTON_CALL_DOWN, i, (int) floors[i].downOrder);
	}
	for (int i = FLOOR_1; i <= FLOOR_3; i++) {
		elev_set_button_lamp(BUTTON_CALL_UP, i, (int) floors[i].upOrder);
	}
	for (int i = FLOOR_1; i <= FLOOR_4; i++) {
		elev_set_button_lamp(BUTTON_COMMAND, i, (int) floors[i].innerOrder);
	}
}

bool getStopButton() {
	bool active = elev_get_stop_signal();
	elev_set_stop_lamp((int) active);
	return active;
}

void setDoorOpen(bool status) {
	elev_set_door_open_lamp((int) status);
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
